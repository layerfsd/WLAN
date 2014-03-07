/* func.c contains the device functions */

#include <linux/mm.h>
#include <linux/fs.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <asm/uaccess.h>

#include "dk.h"
#include "dk_ioctl.h"
#include "client.h"


static int dk_open
(	
 	struct inode *inode, 
	struct file *file
)
{
		int minor;
		int major;
		int error;
#ifdef DK_DEBUG
		printk("DK:: dk_open \n");
#endif
		major = MAJOR(inode->i_rdev);
		minor = MINOR(inode->i_rdev);
		minor = minor & 0x0f;
		
#ifdef DK_DEBUG
		printk("DK:: dk_open:minor=%d\n", minor);
#endif
		/* 
		 * register client returns the cli id 
		 * or error value
		 */ 
		error = register_client(major, minor);
		if (error < 0) {
				return error;
		}

		/*
		 * Store the client id in the private data field
		 */
		 
		file->private_data = (void *)error;

		return 0;
}

static int dk_release
(	
 	struct inode *inode, 
	struct file *file
)
{
		int cli_id;
#ifdef DK_DEBUG
		printk("DK:: dk_release \n");
#endif
		cli_id = (int) file->private_data;

		unregister_client(cli_id);
		
		return 0;
}

static int dk_mmap
(
 	struct file *file,
	struct vm_area_struct *vma
)
{
		unsigned long offset;
		unsigned long prot;

		offset = vma->vm_pgoff << PAGE_SHIFT;

#ifdef DK_DEBUG
		printk("DK::mmap offset = %x \n",(unsigned int)offset);
#endif

		if (offset >= (unsigned long)high_memory) {
			vma->vm_flags |= VM_IO;
		}
		vma->vm_flags |= VM_RESERVED | VM_LOCKED; 

		prot = pgprot_val(vma->vm_page_prot);
		if (boot_cpu_data.x86 > 3) {
				prot |= _PAGE_PCD | _PAGE_PWT;
		}
	    vma->vm_page_prot = __pgprot(prot);

		if (remap_page_range(vma->vm_start,offset,
							 vma->vm_end-vma->vm_start, vma->vm_page_prot)) {
				return -EAGAIN;
		}

		return 0; 
}


static int dk_ioctl
(
 	struct inode *inode, 
	struct file *file,
	unsigned int cmd,
	unsigned long arg
)
{
		INT32 ret=-1;
		INT32 data;
		struct cfg_op co;
		INT32 cli_id;
		INT32 i;
		struct client_info ci;
		struct event_op eo;
		event_handle evt_hnd;
		p_event_struct p_event;
		p_atheros_dev p_client;
		
#ifdef DK_DEBUG
		printk("DK::dk_ioctl \n");
#endif

		cli_id = (int) file->private_data;
		p_client = get_client(cli_id);
		if (p_client == NULL) {
				printk("DK:: Invalid client \n");
				return -1;
		}

		switch (cmd) {
			case DK_IOCTL_GET_VERSION:
				data = (DRV_MAJOR_VERSION << 16) | (DRV_MINOR_VERSION);
				ret = put_user(data, (INT32 *)arg);
				break;
			case DK_IOCTL_GET_CLIENT_INFO:
				if (get_cli_info(cli_id,&ci) < 0) {
					ret = -1;
				} else {
					ret = copy_to_user((void *)arg,(void *)&ci,sizeof(ci));
				}
				ret = 0;
				break;
			case DK_IOCTL_CFG_READ:
				if (copy_from_user((void *)&co,(void *)arg,sizeof(co))) {
						return -EFAULT;
				}
#ifdef DK_DEBUG
				printk("DK::Cfg read @ offset %x \n",co.offset);
#endif
				if (cli_cfg_read(cli_id,co.offset,co.size,&co.value) < 0) {
					ret = -1;
				} else {
					ret = copy_to_user((void *)arg,(void *)&co,sizeof(co));
				}
				break;
			case DK_IOCTL_CFG_WRITE:
				if (copy_from_user((void *)&co,(void *)arg,sizeof(co))) {
						return -EFAULT;
				}
#ifdef DK_DEBUG
				printk("DK::Cfg write @ offset %x : %x \n",co.offset,co.value);
#endif
				if (cli_cfg_write(cli_id,co.offset,co.size,co.value) < 0) {
					ret = -1;
				} else {
					ret = 0;
				}
				break;
			case DK_IOCTL_CREATE_EVENT:
#ifdef DK_DEBUG
				printk("DK::Create event \n");
#endif
				if (copy_from_user((void *)&eo,(void *)arg,sizeof(eo))) {
						return -EFAULT;
				}
				ret = -1;
				if (eo.valid) {
			 		evt_hnd.eventID = eo.param[5] & 0xffff;
					evt_hnd.f2Handle = (eo.param[5] >> 16) & 0xffff;
					p_event = createEvent (eo.param[0], // type
					                       eo.param[1], // persistent
					                       eo.param[2], // param1
					                       eo.param[3], // param2
					                       eo.param[4], // param3
					                       evt_hnd);
					if (p_event != NULL) {
						// need to look at the event type to see which queue
						switch (p_event->type ) {
							case ISR_INTERRUPT:
								//if param1 is zero, we, by default
								// set the "ISR IMR" to pass everything
								if ( 0 == p_event->param1 ) {
									p_event->param1 = 0xffffffff;
								}
								if (pushEvent(p_event, &p_client->isr_event_q,
								               TRUE) ) {
										ret = 0;
								} else {
									printk("DK::Push Event Failed \n");
									kfree(p_event);
								} 
								break;
							default:
								printk("DK::Event Type %d not supported \n",p_event->type);
								kfree(p_event);
								break;
						}
					} 
				}
				break;
			case DK_IOCTL_GET_NEXT_EVENT:
#ifdef DK_DEBUG
				printk("DK::Get next event \n");
#endif
				ret = 0;
				eo.valid = 0;
				if (p_client->trigered_event_q.queueSize) {
					if (checkForEvents(&p_client->trigered_event_q,TRUE)){ 
						p_event = popEvent(&p_client->trigered_event_q,TRUE);
						eo.valid = 1;
						eo.param[0] = p_event->type;
						eo.param[1] = p_event->persistent;
						eo.param[2] = p_event->param1; 
						eo.param[3] = p_event->param2;
						eo.param[4] = p_event->param3;
						eo.param[5] = (p_event->eventHandle.f2Handle << 16) | 
						               p_event->eventHandle.eventID; 
						for (i=0;i<6;i++) { 
							eo.param[6+i] = p_event->result[i]; 
						} 
					#ifdef DK_DEBUG 
						printk("DK:: Pop event %x \n",(UINT32)p_event);
					#endif 
						kfree(p_event);
					} 
				}
				ret = copy_to_user((void *)arg,(void *)&eo,sizeof(eo));
				break;
			default:
				printk("DK::Unreconginzed ioctl command %d \n",cmd);
				break;
		}
		return ret;
}


static struct file_operations dk_fops = {
	owner:	THIS_MODULE,
	open:	dk_open,
	release: dk_release,
	mmap:	dk_mmap,
	ioctl  : dk_ioctl
};

INT32  dk_dev_init(void) {
		int status;
		status = register_chrdev(DK_MAJOR_NUMBER,"dk",&dk_fops);
		printk("dk_dev_init::status after register_chrdev(dk) = %d\n", status);

#ifdef DK_UART
	        status |= register_chrdev(DK_UART_MAJOR_NUMBER, "dk_uart", &dk_fops);
		printk("dk_dev_init::status after register_chrdev(dk_uart) = %d\n", status);
#endif
		return status;
}

void dk_dev_exit(void) {
		unregister_chrdev(DK_MAJOR_NUMBER,"dk");
//#ifdef DK_UART
		unregister_chrdev(DK_UART_MAJOR_NUMBER,"dk_uart");
//#endif
}
