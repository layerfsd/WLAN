/* client.c */

#include <linux/pci.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/ioport.h>
#include <linux/vmalloc.h>
#include <asm/io.h>
#include <linux/wrapper.h>

#ifdef PCI_BUS
#include "dk_pci_bus.h"
#endif

#include "dk.h"
#include "dk_ioctl.h"
#include "dk_event.h"
#include "client.h"

#define ORDER 9
#define ORDER_COMM_CLASS 8
#define ATHEROS_VENDOR_ID 0x168c

/* global variables */
static atheros_dev dev_table[MAX_CLIENTS_SUPPORTED];
static UINT32 mem_table[MAX_CLIENTS_SUPPORTED];
static UINT32 mem_page_order[MAX_CLIENTS_SUPPORTED];

/* extern declarations */
extern void dk_intr_handler(INT32,VOID *,struct pt_regs *);

/* forward declarations */
static VOID reset_device(INT32 cli_id);

VOID init_client
(
 	VOID
)
{
	UINT32 i;

#ifdef DK_DEBUG
	printk("DK::Init dev table \n");
#endif

	for (i=0;i<MAX_CLIENTS_SUPPORTED;i++) {
			dev_table[i].cli_id = INVALID_CLIENT;
			mem_table[i] = 0;
	}
		
}

static VOID init_atheros_dev
(
 	p_atheros_dev dev
)
{
        int iIndex;

		dev->bus_dev = NULL;
		dev->reg_phy_addr = 0;
		dev->reg_ker_vir_addr = 0;
		dev->reg_range = 0;
		dev->mem_phy_addr = 0;
		dev->mem_ker_vir_addr = 0;
		dev->mem_size = 0;
		dev->irq = 0;
		dev->dev_busy = 0;
		dev->cli_id = INVALID_CLIENT;
		memset(&dev->isr_event_q,0,sizeof(event_queue));
		memset(&dev->trigered_event_q,0,sizeof(event_queue));
        for(iIndex=0; iIndex<MAX_BARS; iIndex++) {
		     dev->areg_phy_addr[iIndex] = 0;
		     dev->areg_ker_vir_addr[iIndex] = 0;
		     dev->areg_range[iIndex] = 0;
        }
}

/*
 * Get the client for this device
 * The table give the mapping between the device and the client 
 * in the dev_table. The minor number is used to idenitfy the client
 * for the device.
 *  Device    client       minor 
 * /dev/dk0 - dev_table[0]   0
 * /dev/dk1 - dev_table[1]   1
 * /dev/dk2 - dev_table[2]   2
 * /dev/dk3 - dev_table[3]   3
 */
static INT32 get_client_id
(
 	INT32 major,
 	INT32 minor
)
{
#ifdef DK_DEBUG
	printk("DK::get_client_id:major=%d:minor=%d\n", major, minor);
#endif
	if (major == DK_UART_MAJOR_NUMBER) {
		return dev_table[minor + UART_FN_DEV_START_NUM].cli_id;
	}
	else
		return dev_table[minor].cli_id;
}

p_atheros_dev get_client
(
 	INT32 cli_id
)
{
	p_atheros_dev dev;

	dev = &dev_table[cli_id];
	if (!VALID_CLIENT(dev))  {
		return NULL;
	}

	return dev;
}

INT32 add_client
(
 	VOID *bus_dev,
	UINT32 baseaddr[MAX_BARS],
	UINT32 len[MAX_BARS],
	UINT32 irq,
    UINT32 numBars,
    UINT32 sIndex
)
{
		UINT32 i;
		INT32 cli_id;
		p_atheros_dev dev;
		UINT32 reg_vir_addr[MAX_BARS];
		UINT32 mem_phy_addr;
		UINT32 *mem;
		struct page *page;
		UINT32 no_pages, iIndex;
        INT8 ret_val;
        UINT32 page_order=ORDER;

		dev = NULL;
		mem = NULL;
		cli_id = INVALID_CLIENT;
		mem_phy_addr = 0;

		for (i=sIndex;i<MAX_CLIENTS_SUPPORTED;i++) {
				if (dev_table[i].cli_id == INVALID_CLIENT) {
						cli_id = i;
						dev = &dev_table[i];
						mem = &mem_table[i];
						break;
				}
		}

		if (cli_id == INVALID_CLIENT) {
			printk("DK:: Only %d clients supported \n",MAX_CLIENTS_SUPPORTED);
			return -1;
		}
		
		printk("DK::Add client %d \n",cli_id);

		init_atheros_dev(dev);		

        for(iIndex=0; iIndex<numBars; iIndex++) {
            (void)pci_read_config_byte(bus_dev, PCI_BASE_ADDRESS_0 + (iIndex *4), &ret_val);
            if (ret_val&0x1) {
               // IO region mapping
               /*
		       printk("DK:: Requesting IO region=%x:range=%d\n", baseaddr[iIndex], len[iIndex]);
		       if (request_region(baseaddr[iIndex],len[iIndex],DRV_NAME) == NULL) {
		          printk(KERN_ERR "DK:: unable to request io region for Bar %d: len = %d\n", iIndex, len[iIndex]);
				  return -1;
	           }
               */
            }
            else {
		      printk("DK:: Requesting MEM region=%x:range=%d\n", baseaddr[iIndex], len[iIndex]);
		      if (request_mem_region(baseaddr[iIndex],len[iIndex],DRV_NAME) == NULL) {
		          printk(KERN_ERR "DK:: unable to request mem region for Bar %d: len = %d\n", iIndex, len[iIndex]);
				  return -1;
	          }
		      reg_vir_addr[iIndex] = (UINT32)ioremap_nocache(baseaddr[iIndex],len[iIndex]);
		      if ((VOID *)reg_vir_addr[iIndex] == NULL) {
				free_irq(irq,(void *)dev);
                for(iIndex=0; iIndex<numBars; iIndex++) {
				  iounmap((VOID *)reg_vir_addr[iIndex]);
			  	  release_mem_region(baseaddr[iIndex],len[iIndex]);
                }
				printk(KERN_ERR "DK:: unable to remap registers \n");
				return -1;
		      }
           }
        } // end of for

		   /* Pass the pointer to the client id */
		   if (request_irq(irq,dk_intr_handler,SA_SHIRQ,DRV_NAME,(void *)dev) != 0) {
              for(iIndex=0; iIndex<numBars; iIndex++) {
				iounmap((VOID *)reg_vir_addr[iIndex]);
				release_mem_region(baseaddr[iIndex],len[iIndex]);
              }
		        printk(KERN_ERR "DK:: unable to request irq \n");
				return -1;
		   }


        for(iIndex=0; iIndex<numBars; iIndex++) {
		   printk("DK::Reg phy addr = %x vir Addr = %x \n",baseaddr[iIndex],reg_vir_addr[iIndex]);
        }
		printk("DK::Irq = %x \n",irq);
		
		if (*mem == 0) {
                (void)pci_read_config_byte(bus_dev, 0xb, &ret_val);
                if (ret_val == SIMPLE_COMM_CLASS) {
				    *mem = __get_free_pages(GFP_KERNEL,ORDER_COMM_CLASS);
                    page_order = ORDER_COMM_CLASS;
                }
                else {
				    *mem = __get_free_pages(GFP_KERNEL,ORDER);
                    page_order = ORDER;
                }
				if (*mem == 0) {
                    for(iIndex=0; iIndex<numBars; iIndex++) {
					   iounmap((VOID *)reg_vir_addr[iIndex]);
					   free_irq(irq,(void *)dev);
					   release_mem_region(baseaddr[iIndex],len[iIndex]);
                    }
					   printk(KERN_ERR "DK:: unable to allocate contigous memory \n");
					   return -1;
				}
				// map the pages as reserved,
				// otherwise remap_page_range wont 
				// do what we expect
				page = virt_to_page(*mem);
				no_pages = 1 << page_order;
                mem_page_order[cli_id] = page_order;
				for (i=0;i<no_pages;i++) {
					mem_map_reserve(page);
					page++;
				}
				
				printk("DK::Allocated 0x%x bytes memory \n", PAGE_SIZE * (1<<page_order));
		} else {
				printk("DK::Using memory already allocated at %x \n",*mem);
		}
		mem_phy_addr = __pa(*mem);
		printk("DK::memory vir Addr = %x phy Addr = %x \n",*mem,mem_phy_addr);

		/* 
		 * TO DO: (if necessary)
		 * Kernel already has a mapping for this physical pages. 
		 * They are part of the identity mapping, so they may be 
		 * cached. Remap it to another virtual address space 
		 * if the memory will be accessed from the kernel
		 */
		
		dev->bus_dev = bus_dev;
        for(iIndex=0; iIndex<numBars; iIndex++) {
		   dev->areg_phy_addr[iIndex] = baseaddr[iIndex];
		   dev->areg_ker_vir_addr[iIndex] = reg_vir_addr[iIndex];
		   dev->areg_range[iIndex] = len[iIndex];
        }
		dev->reg_phy_addr = baseaddr[0];
		dev->reg_ker_vir_addr = reg_vir_addr[0];
		dev->reg_range = len[0];
        dev->numBars = numBars;
		dev->mem_ker_vir_addr = *mem;
		dev->mem_phy_addr = mem_phy_addr;
		dev->mem_size = (1 << page_order) * PAGE_SIZE;
		dev->irq = irq;
		dev->cli_id = cli_id;

		cli_cfg_read(cli_id,8,4,&dev->device_class);
        dev->device_class >>= 24;
		reset_device(cli_id);
		
		return 0;
}


VOID remove_client
(
 	VOID *bus_dev
)
{
		UINT32 i, iIndex;
		p_atheros_dev dev;

		printk("DK::Remove client \n");

		dev = NULL;
		for (i=0;i<MAX_CLIENTS_SUPPORTED;i++) {
				if (dev_table[i].cli_id == INVALID_CLIENT) continue;
			   	if (dev_table[i].bus_dev == bus_dev) {
						dev = &dev_table[i];
						break;
				}
		}
		
		if (dev == NULL) {
			printk("DK:: Invalid client in remove_client \n");
			return;
		}
		
		reset_device(dev->cli_id);

        for(iIndex=0; iIndex<dev->numBars; iIndex++) {
		    iounmap((VOID *)dev->areg_ker_vir_addr[iIndex]);
		    release_mem_region(dev->areg_phy_addr[iIndex], dev->areg_range[iIndex]);
		    printk("DK::Free reg space phy=%x vir=%x \n",dev->areg_phy_addr[iIndex],dev->areg_ker_vir_addr[iIndex]);
        }
		
		free_irq(dev->irq,(void *)dev);
		printk("DK::Free irq = %x \n",dev->irq);

		init_atheros_dev(dev);		

		return;
}


VOID cleanup_client
(
 	VOID
)
{
		UINT32 i;
		UINT32 j;
		struct page *page;
		UINT32 no_pages;

#ifdef DK_DEBUG
		printk("DK::Cleanup dev table \n");
#endif // DK_DEBUG
		for (i=0;i<MAX_CLIENTS_SUPPORTED;i++) {
				if (dev_table[i].cli_id != INVALID_CLIENT) {
						remove_client(dev_table[i].bus_dev);
						dev_table[i].cli_id = INVALID_CLIENT;
				}
				if (mem_table[i] != 0) {
						printk("DK::Freeing memory at %x \n",mem_table[i]);
						// remove the reserved flag
						
						page = virt_to_page(mem_table[i]);
						no_pages = 1 << mem_page_order[i];
                        printk("page order=%d\n", mem_page_order[i]);
						for (j=0;j<no_pages;j++) {
							mem_map_unreserve(page);
							page++;
						}
						
						free_pages(mem_table[i],8);
						mem_table[i] = 0;
				}
		}
		return;
}

INT32 register_client
(
 	INT32 major,
 	INT32 minor
)
{
	INT32 cli_id;
	p_atheros_dev dev;
	UINT32 vendor_id;

	/* get the client for this device */
	cli_id = get_client_id(major, minor);

	if (cli_id == INVALID_CLIENT) {
		printk("DK::register_client:Device not found \n");
	 	return -ENODEV;
	}

	printk("DK::Regsitering client %d \n",cli_id);
	dev = &dev_table[cli_id];

	if (!VALID_CLIENT(dev)) {
		printk("DK::register_client:Invalid client \n");
	 	return -ENODEV;
    }

	if (BUSY_CLIENT(dev)) {
		printk("DK::register_client:Client alreay in use \n");
		return -EACCES;
	}
	
	// check whether the device is present
	// by reading the vendor id
	cli_cfg_read(cli_id,0,4,&vendor_id);
	if ((vendor_id & 0xffff) != ATHEROS_VENDOR_ID) {
		printk("DK::Device not present \n");
	 	return -ENODEV;
	}

	if (bus_dev_init(dev->bus_dev) < 0) {
		printk("DK::register_client:Cannot initialize client \n");
		return -EACCES;
	}

	initEventQueue(&dev->isr_event_q);
	initEventQueue(&dev->trigered_event_q);
	
	reset_device(cli_id);
					
	dev->dev_busy = 1;
				  
	return dev->cli_id;
}

VOID unregister_client
(
 	INT32 cli_id
)
{
	p_atheros_dev dev;

	printk("DK::Unregsitering client %d \n",cli_id);
	
	dev = &dev_table[cli_id];

	if (!VALID_CLIENT(dev)) {
		printk("DK::unregister_client:Invalid client \n");
	 	return;
    }
	if (!BUSY_CLIENT(dev)) {
		printk("DK::unregister_client:Client not registered \n");
		return;
	}
	
	reset_device(dev->cli_id);
	
	deleteEventQueue(&dev->isr_event_q);
	deleteEventQueue(&dev->trigered_event_q);
	
	bus_dev_exit(dev->bus_dev);
	dev->dev_busy = 0;
	
	return;
}

INT32 cli_reg_read
(
 	INT32 cli_id,
	INT32 offset,
	UINT32 *data
)
{
	p_atheros_dev dev;
	UINT32 *addr;
	
	dev = &dev_table[cli_id];
	if (!VALID_CLIENT(dev)) {
		printk("DK::reg_read:Invalid client \n");
	 	return -1;
    }
	
	addr = (UINT32 *)(dev->areg_ker_vir_addr[0] + offset);
	*data = *addr;
#ifdef DK_DEBUG
	printk("DK::Reg read @ %x : %x \n",(UINT32)addr,*data);
#endif

	return 0;
}

INT32 cli_reg_write
(
 	INT32 cli_id,
	INT32 offset,
	UINT32 data
)
{
	p_atheros_dev dev;
	UINT32 *addr;

	dev = &dev_table[cli_id];
	if (!VALID_CLIENT(dev)) {
		printk("DK::reg_write:Invalid client \n");
	 	return -1;
    }
	
	addr = (UINT32 *)(dev->areg_ker_vir_addr[0] + offset);
	*addr = data;
#ifdef DK_DEBUG
	printk("DK::Reg write @ %x : %x \n",(UINT32)addr,data);
#endif

	return 0;
}

INT32 cli_cfg_read
(
 	INT32 cli_id,
	INT32 offset,
	INT32 size,
	INT32 *ret_val
)
{
	p_atheros_dev dev;

	dev = &dev_table[cli_id];
	
	if (!VALID_CLIENT(dev)) {
		printk("DK::cfg_read:Invalid client \n");
	 	return -1;
    }
	
	return bus_cfg_read(dev->bus_dev,offset,size,ret_val);
}

INT32 cli_cfg_write
(
 	INT32 cli_id,
	INT32 offset,
	INT32 size,
	INT32 ret_val
)
{
	p_atheros_dev dev;
	
	dev = &dev_table[cli_id];
	
	if (!VALID_CLIENT(dev)) {
		printk("DK::cfg_write:Invalid client \n");
	 	return -1;
    }
	
	return bus_cfg_write(dev->bus_dev,offset,size,ret_val);
}

INT32 get_cli_info
(
 	INT32 cli_id,
	struct client_info *ci
)
{
	p_atheros_dev dev;
    int iIndex;
	
	dev = &dev_table[cli_id];
	
	if (!VALID_CLIENT(dev)) {
		printk("DK::get_client_info:Invalid client \n");
	 	return -1;
    }
	
	ci->reg_phy_addr = dev->areg_phy_addr[0];
	ci->reg_range = dev->areg_range[0];
    for (iIndex=0; iIndex<dev->numBars; iIndex++) {
	   ci->areg_phy_addr[iIndex] = dev->areg_phy_addr[iIndex];
	   ci->areg_range[iIndex] = dev->areg_range[iIndex];
    }
    ci->numBars = dev->numBars;
	ci->mem_phy_addr = dev->mem_phy_addr;
	ci->mem_size = dev->mem_size;
	ci->irq = dev->irq;
    ci->device_class = dev->device_class;

	return 0;
}

static VOID reset_device
(
 	INT32 cli_id
)
{
#ifdef DK_DEBUG
	printk("DK::Disable the interrupts and reset the device \n");	
#endif
	if (dev_table[cli_id].device_class == NETWORK_CLASS) {
	   // disable interrupts 	
	   cli_reg_write(cli_id,0x0024,0x0);
	   // put the device in reset state		
	   cli_reg_write(cli_id,0x4000,0x1f); 				
	}
	if (dev_table[cli_id].device_class == SIMPLE_COMM_CLASS) {
	   cli_reg_write(cli_id,0x4,0x0); // IER
	   cli_reg_write(cli_id,0x114,0x0); // Extended IER
	   cli_reg_write(cli_id,0x104,0x1); // RC
	}


	return;
}	
