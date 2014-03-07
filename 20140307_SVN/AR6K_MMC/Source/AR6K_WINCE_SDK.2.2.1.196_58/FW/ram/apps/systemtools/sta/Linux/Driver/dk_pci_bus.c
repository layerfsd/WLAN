/* dk_pci_bus.c - contains functions specific to pci bus */

#include <linux/pci.h>
#include <linux/module.h>
#include <linux/version.h>
#include "dk.h"
#include "client.h"

#define ATHEROS_VENDOR_ID 0x168c
#define MAX_CFG_OFFSET	256

static INT32 dk_pci_probe
(
 	struct pci_dev *dev, 
	const struct pci_device_id *id
)
{
	INT32 error;
	UINT32 baseaddr[MAX_BARS];
	UINT32 len[MAX_BARS];
	UINT32 irq;
	UINT32 iIndex, numBars;
#ifdef DK_DEBUG
    UINT16 device_id, vendor_id;
#endif
	INT8 ret_val;
	UINT32 sIndex = WMAC_FN_DEV_START_NUM;

#ifdef DK_DEBUG
	printk("DK::Pci probe \n");
#endif
	
	error = pci_enable_device(dev); 
	if (error != 0) {
			printk(KERN_ERR "DK:: pci_enable_device failed \n");
			return error;
	}


    for (iIndex=0; iIndex<MAX_BARS; iIndex++) {
	  baseaddr[iIndex] = pci_resource_start(dev,iIndex);
	  len[iIndex] = pci_resource_len(dev,iIndex);
      if (len[iIndex] == 0) break;
    }
    numBars = iIndex;
#ifdef DK_DEBUG
	printk("DK::num bars = %d\n", numBars);
	(void) pci_read_config_word(dev, PCI_VENDOR_ID, &vendor_id); 
	(void) pci_read_config_word(dev, PCI_DEVICE_ID, &device_id); 
    printk("DK::Vendor Id=%x:Device id = %x\n", vendor_id, device_id);
#endif
	irq = dev->irq;
	
    for (iIndex=0; iIndex<numBars; iIndex++) {
	   pci_write_config_dword(dev,PCI_BASE_ADDRESS_0 + (iIndex *4), baseaddr[iIndex]);
	   pci_write_config_byte(dev,PCI_INTERRUPT_LINE, irq);
    }
	
	(void) pci_read_config_byte(dev, 0xb, &ret_val); // Get the class code
	if (ret_val == NETWORK_CLASS) {
		sIndex = WMAC_FN_DEV_START_NUM;
	}
	if (ret_val == SIMPLE_COMM_CLASS) {
		sIndex = UART_FN_DEV_START_NUM;
	}
	printk("Class code = %d:start search index=%d\n", ret_val, sIndex);
	
	if (add_client(dev,baseaddr,len,irq, numBars, sIndex) < 0) {
		printk(KERN_ERR "DK:: unable to add client \n");
#if LINUX_VERSION_CODE > 132098
		pci_disable_device(dev);
#endif
		return -ENODEV;
	}
	

	return 0;
}

static VOID dk_pci_remove
(
	struct pci_dev *dev
)
{
#ifdef DK_DEBUG
	printk("DK::Pci remove \n");
#endif
	remove_client(dev);
#if LINUX_VERSION_CODE > 132098
	pci_disable_device(dev);
#endif
	
	return;
}

#if LINUX_VERSION_CODE > 132098
static INT32 dk_pci_suspend
(
 	struct pci_dev *dev,
	UINT32 state
)
#else
static VOID dk_pci_suspend
(
 	struct pci_dev *dev
)
#endif
{
#ifdef DK_DEBUG
		printk("DK::Pci suspend \n");
#endif
#if LINUX_VERSION_CODE > 132098
		return 0;
#else
		return;
#endif
}

#if LINUX_VERSION_CODE > 132098
static INT32 dk_pci_resume
(
	struct pci_dev *dev
)
#else
static VOID dk_pci_resume
(
	struct pci_dev *dev
)
#endif
{
#ifdef DK_DEBUG
		printk("DK::Pci resume \n");
#endif
#if LINUX_VERSION_CODE > 132098
		return 0;
#else
		return;
#endif
}

#if LINUX_VERSION_CODE > 132098
static INT32 dk_pci_save_state
(
 	struct pci_dev *dev,
	UINT32 state
)
{
#ifdef DK_DEBUG
		printk("DK::Pci save state \n");
#endif
		return 0;
}

static INT32 dk_pci_enable_wake
(
 	struct pci_dev *dev,
	UINT32 state,
	INT32 enable
)
{
#ifdef DK_DEBUG
		printk("DK::Pci enable wake \n");
#endif
		return 0;
}
#endif

static struct pci_device_id __devinitdata dk_id_tbl[] = {
	{ATHEROS_VENDOR_ID, 0x0011, PCI_ANY_ID, PCI_ANY_ID, 0, 0, (unsigned long)"MAUI"},
	{ATHEROS_VENDOR_ID, 0x0012, PCI_ANY_ID, PCI_ANY_ID, 0, 0, (unsigned long)"OAHU"},
	{ATHEROS_VENDOR_ID, 0x0013, PCI_ANY_ID, PCI_ANY_ID, 0, 0, (unsigned long)"VENICE"},
	{ATHEROS_VENDOR_ID, 0x0014, PCI_ANY_ID, PCI_ANY_ID, 0, 0, (unsigned long)"VENICE_DERBY"},
	{ATHEROS_VENDOR_ID, 0xff16, PCI_ANY_ID, PCI_ANY_ID, 0, 0, (unsigned long)"GRIFFIN_MAC"},
	{0,}
};

								
#ifdef DK_UART
static struct pci_device_id __devinitdata dk_uart_id_tbl[] = {
	{ATHEROS_VENDOR_ID, 0xff96, PCI_ANY_ID, PCI_ANY_ID, 0, 0, (unsigned long)"GRIFFIN_UART"},
	{0,}
};
#endif

static struct pci_driver dkpci_driver = {
		name:	"dkkernel",
		id_table: dk_id_tbl,
		probe: dk_pci_probe,
		remove: dk_pci_remove,
		suspend: dk_pci_suspend,
		resume: dk_pci_resume,
#if LINUX_VERSION_CODE > 132098
		save_state: dk_pci_save_state,
		enable_wake: dk_pci_enable_wake
#endif
};

#ifdef DK_UART

static struct pci_driver dkuart_pci_driver = {
		name:	"dkuartkernel",
		id_table: dk_uart_id_tbl,
		probe: dk_pci_probe,
		remove: dk_pci_remove,
		suspend: dk_pci_suspend,
		resume: dk_pci_resume,
#if LINUX_VERSION_CODE > 132098
		save_state: dk_pci_save_state,
		enable_wake: dk_pci_enable_wake
#endif
};

#endif
INT32 bus_module_init
(
 	VOID
) 
{
	int status;
#ifdef DK_DEBUG
	printk("DK::Bus module init  \n");	
#endif // DK_DEBUG
#ifdef MODULE
#ifdef DK_DEBUG
	printk("DK::MODULE\n");	
#endif // DK_DEBUG
	status = pci_module_init(&dkpci_driver);
	printk("DK::bus_module_init:status=%d from dkpci_driver\n", status);
#ifdef DK_UART
	status |= pci_module_init(&dkuart_pci_driver);
	printk("DK::bus_module_init:status=%d from dkuart_pci_driver\n", status);
#endif
	return status;
#else
	return pci_register_driver(&dkpci_driver);
#endif // MODULE
}

VOID bus_module_exit
(
 	VOID
)
{
#ifdef DK_DEBUG
	printk("DK::Bus module exit  \n");	
#endif // DK_DEBUG
	pci_unregister_driver(&dkpci_driver);
#ifdef DK_UART
	pci_unregister_driver(&dkuart_pci_driver);
#endif
}

#ifdef MODULE
MODULE_DEVICE_TABLE(pci,dk_id_tbl);
#endif

INT32 bus_dev_init
(
 	void  *bus_dev
)
{
	struct pci_dev *dev;
	UINT32 baseaddr;
	UINT32 irq;
	
	dev = (struct pci_dev *)bus_dev;
	pci_set_master(dev);
	
	baseaddr = pci_resource_start(dev,0);
	irq = dev->irq;

	/*
	 * Program the base address and irq as the device may 
	 * be hotplugged without aware of the kernel
	 */
	
	pci_write_config_dword(dev,PCI_BASE_ADDRESS_0, baseaddr);
	pci_write_config_byte(dev,PCI_INTERRUPT_LINE, irq);

	return 0;
}

VOID bus_dev_exit
(
 	void  *bus_dev
)
{
	return;
}

INT32 bus_cfg_read
(
 	void  *bus_dev,
 	INT32 offset,
	INT32 size,
	INT32 *ret_val
)
{
	struct pci_dev *dev;
	int ret =  -1;
	
	dev = (struct pci_dev *)bus_dev;

	if (size < MAX_CFG_OFFSET) {
		switch (size) {
			case 1:
				ret = pci_read_config_byte(dev,offset, (INT8 *)ret_val);
				break;
			case 2:
				offset = offset & 0xfe;
				ret = pci_read_config_word(dev,offset, (INT16 *)ret_val);
				break;
			case 4:
				offset = offset & 0xfc;
				ret = pci_read_config_dword(dev,offset, ret_val);
				break;
			default:
				break;
		}
	}

	return ret;

}

INT32 bus_cfg_write
(
 	void  *bus_dev,
 	INT32 offset,
	INT32 size,
	INT32 val
)
{
	struct pci_dev *dev;
	int ret =  -1;
	
	dev = (struct pci_dev *)bus_dev;
	if (size < MAX_CFG_OFFSET) {
		switch (size) {
			case 1:
				ret = pci_write_config_byte(dev,offset, val);
				break;
			case 2:
				offset = offset & 0xfe;
				ret = pci_write_config_word(dev,offset, val);
				break;
			case 4:
				offset = offset & 0xfc;
				ret = pci_write_config_dword(dev,offset,val);
				break;
			default:
				break;
		}
	}

	return ret;
}
