/* main.c main 'C' file for the linux dk driver */

/* Copyright (c) 2001 Atheros Communications, Inc., All Rights Reserved */

// Include files
#include <linux/version.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/errno.h>

#include "dk.h"
#include "client.h"
#ifdef PCI_BUS
#include "dk_pci_bus.h"
#endif

extern INT32  dk_dev_init(void);
extern void dk_dev_exit(void);

static INT32 __init dk_module_init(void) 
{
		INT32 error;
		
#ifdef DK_DEBUG
		printk("DK::Module init \n");
#endif // DK_DEBUG
		error = dk_dev_init();
		if (error < 0) {
			printk("DK::Cannot register device \n");
			return error;
		}
		init_client();
		error = bus_module_init();
		if (error < 0) {
			cleanup_client();
			dk_dev_exit();
			printk("DK::Cannot locate device. Reset the machine \n");
			return error;
		}
		return 0;
}

static void __exit  dk_module_exit(void)
{
#ifdef DK_DEBUG
		printk("DK::Module exit \n");
#endif // DK_DEBUG
		bus_module_exit();
		cleanup_client();
		dk_dev_exit();

		return;
}

#ifdef MODULE

#if LINUX_VERSION_CODE > 132098
	MODULE_LICENSE(MOD_LICENCE);
#endif

MODULE_AUTHOR(MOD_AUTHOR);
MODULE_DESCRIPTION(MOD_DESCRIPTION);

module_init(dk_module_init);
module_exit(dk_module_exit);

#endif // MODULE

