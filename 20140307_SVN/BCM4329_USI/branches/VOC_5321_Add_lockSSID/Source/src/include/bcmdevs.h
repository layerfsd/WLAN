/*
 * Broadcom device-specific manifest constants.
 *
 * Copyright (C) 2010, Broadcom Corporation
 * All Rights Reserved.
 * 
 * THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY
 * KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
 * SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
 *
 * $Id: bcmdevs.h,v 13.172.4.5.4.10.2.36 2010/05/25 08:33:44 Exp $
 */

#ifndef	_BCMDEVS_H
#define	_BCMDEVS_H

/* PCI vendor IDs */
#define	VENDOR_EPIGRAM		0xfeda
#define	VENDOR_BROADCOM		0x14e4
#define VENDOR_SI_IMAGE		0x1095		/* Silicon Image, used by Arasan SDIO Host */
#define VENDOR_TI		0x104c		/* Texas Instruments */
#define VENDOR_RICOH		0x1180		/* Ricoh */
#define VENDOR_JMICRON		0x197b
#if !defined(ESTA_POSTMOGRIFY_REMOVAL)
#define	VENDOR_3COM		0x10b7
#define	VENDOR_NETGEAR		0x1385
#define	VENDOR_DIAMOND		0x1092
#define	VENDOR_INTEL		0x8086
#define	VENDOR_DELL		0x1028
#define	VENDOR_HP		0x103c
#define	VENDOR_HP_COMPAQ	0x0e11
#define	VENDOR_APPLE		0x106b
#define VENDOR_BUFFALO		0x1154		/* Buffalo vendor id */

#endif /* !defined(ESTA_POSTMOGRIFY_REMOVAL) */

/* PCMCIA vendor IDs */
#define	VENDOR_BROADCOM_PCMCIA	0x02d0

/* SDIO vendor IDs */
#define	VENDOR_BROADCOM_SDIO	0x00BF

/* DONGLE VID/PIDs */
#define BCM_DNGL_VID            0xa5c
#define BCM_DNGL_BL_PID_4320    0xbd11
#define BCM_DNGL_BL_PID_4328    0xbd12
#define BCM_DNGL_BL_PID_4322    0xbd13
#define BCM_DNGL_BL_PID_4325    0xbd14
#define BCM_DNGL_BL_PID_4315    0xbd15
#define BCM_DNGL_BL_PID_4319    0xbd16
#define BCM_DNGL_BDC_PID        0xbdc

#if !defined(ESTA_POSTMOGRIFY_REMOVAL)
/* PCI Device IDs */
#define	BCM4210_DEVICE_ID	0x1072		/* never used */
#define	BCM4230_DEVICE_ID	0x1086		/* never used */
#define	BCM4401_ENET_ID		0x170c		/* 4401b0 production enet cards */
#define	BCM3352_DEVICE_ID	0x3352		/* bcm3352 device id */
#define	BCM3360_DEVICE_ID	0x3360		/* bcm3360 device id */
#define	BCM4211_DEVICE_ID	0x4211
#define	BCM4231_DEVICE_ID	0x4231
#define	BCM4303_D11B_ID		0x4303		/* 4303 802.11b */
#define	BCM4311_D11G_ID		0x4311		/* 4311 802.11b/g id */
#define	BCM4311_D11DUAL_ID	0x4312		/* 4311 802.11a/b/g id */
#define	BCM4311_D11A_ID		0x4313		/* 4311 802.11a id */
#define	BCM4328_D11DUAL_ID	0x4314		/* 4328 802.11a/g id */
#define	BCM4328_D11G_ID		0x4315		/* 4328 802.11g id */
#define	BCM4328_D11A_ID		0x4316		/* 4328 802.11a id */
#define	BCM4318_D11G_ID		0x4318		/* 4318 802.11b/g id */
#define	BCM4318_D11DUAL_ID	0x4319		/* 4318 802.11a/b/g id */
#define	BCM4318_D11A_ID		0x431a		/* 4318 802.11a id */
#endif /* !defined(ESTA_POSTMOGRIFY_REMOVAL) */
#define	BCM4325_D11DUAL_ID	0x431b		/* 4325 802.11a/g id */
#define	BCM4325_D11G_ID		0x431c		/* 4325 802.11g id */
#define	BCM4325_D11A_ID		0x431d		/* 4325 802.11a id */
#if !defined(ESTA_POSTMOGRIFY_REMOVAL)
#define	BCM4306_D11G_ID		0x4320		/* 4306 802.11g */
#define	BCM4306_D11A_ID		0x4321		/* 4306 802.11a */
#define	BCM4306_UART_ID		0x4322		/* 4306 uart */
#define	BCM4306_V90_ID		0x4323		/* 4306 v90 codec */
#define	BCM4306_D11DUAL_ID	0x4324		/* 4306 dual A+B */
#define	BCM4306_D11G_ID2	0x4325		/* BCM4306_D11G_ID; INF w/loose binding WAR */
#define	BCM4321_D11N_ID		0x4328		/* 4321 802.11n dualband id */
#define	BCM4321_D11N2G_ID	0x4329		/* 4321 802.11n 2.4Ghz band id */
#define	BCM4321_D11N5G_ID	0x432a		/* 4321 802.11n 5Ghz band id */
#define BCM4322_D11N_ID		0x432b		/* 4322 802.11n dualband device */
#define BCM4322_D11N2G_ID	0x432c		/* 4322 802.11n 2.4GHz device */
#define BCM4322_D11N5G_ID	0x432d		/* 4322 802.11n 5GHz device */
#endif /* !defined(ESTA_POSTMOGRIFY_REMOVAL) */
#define BCM4329_D11NDUAL_ID	0x432e		/* 4329 802.11n dualband device */
#define BCM4329_D11N2G_ID	0x432f		/* 4329 802.11n 2.4G device */
#define BCM4329_D11N5G_ID	0x4330		/* 4329 802.11n 5G device */
#define BCM4336_D11N_ID		0x4343		/* 4336 802.11n 2.4GHz device */
#define	BCM4315_D11DUAL_ID	0x4334		/* 4315 802.11a/g id */
#define	BCM4315_D11G_ID		0x4335		/* 4315 802.11g id */
#define	BCM4315_D11A_ID		0x4336		/* 4315 802.11a id */
#define BCM4319_D11N_ID		0x4337		/* 4319 802.11n dualband device */
#define BCM4319_D11N2G_ID	0x4338		/* 4319 802.11n 2.4G device */
#define BCM4319_D11N5G_ID	0x4339		/* 4319 802.11n 5G device */


#if !defined(ESTA_POSTMOGRIFY_REMOVAL)
#define	BCMGPRS_UART_ID		0x4333		/* Uart id used by 4306/gprs card */
#define	BCMGPRS2_UART_ID	0x4344		/* Uart id used by 4306/gprs card */
#define FPGA_JTAGM_ID		0x43f0		/* FPGA jtagm device id */
#define BCM_JTAGM_ID		0x43f1		/* BCM jtagm device id */
#endif /* !defined(ESTA_POSTMOGRIFY_REMOVAL) */
#define SDIOH_FPGA_ID		0x43f2		/* sdio host fpga */
#if !defined(ESTA_POSTMOGRIFY_REMOVAL)
#define BCM_SDIOH_ID		0x43f3		/* BCM sdio host id */
#define SDIOD_FPGA_ID		0x43f4		/* sdio device fpga */
#endif /* !defined(ESTA_POSTMOGRIFY_REMOVAL) */
#define SPIH_FPGA_ID		0x43f5		/* PCI SPI Host Controller FPGA */
#if !defined(ESTA_POSTMOGRIFY_REMOVAL)
#define BCM_SPIH_ID		0x43f6		/* Synopsis SPI Host Controller */
#define MIMO_FPGA_ID		0x43f8		/* FPGA mimo minimacphy device id */
#define	BCM4402_ENET_ID		0x4402		/* 4402 enet */
#define	BCM4402_V90_ID		0x4403		/* 4402 v90 codec */
#define	BCM4410_DEVICE_ID	0x4410		/* bcm44xx family pci iline */
#define	BCM4412_DEVICE_ID	0x4412		/* bcm44xx family pci enet */
#define	BCM4430_DEVICE_ID	0x4430		/* bcm44xx family cardbus iline */
#define	BCM4432_DEVICE_ID	0x4432		/* bcm44xx family cardbus enet */
#define	BCM4704_ENET_ID		0x4706		/* 4704 enet (Use 47XX_ENET_ID instead!) */
#endif /* !defined(ESTA_POSTMOGRIFY_REMOVAL) */
#define	BCM4710_DEVICE_ID	0x4710		/* 4710 primary function 0 */
#if !defined(ESTA_POSTMOGRIFY_REMOVAL)
#define	BCM47XX_ILINE_ID	0x4711		/* 47xx iline20 */
#define	BCM47XX_V90_ID		0x4712		/* 47xx v90 codec */
#define	BCM47XX_ENET_ID		0x4713		/* 47xx enet */
#define	BCM47XX_EXT_ID		0x4714		/* 47xx external i/f */
#define	BCM47XX_USB_ID		0x4715		/* 47xx usb */
#define	BCM47XX_USBH_ID		0x4716		/* 47xx usb host */
#define	BCM47XX_USBD_ID		0x4717		/* 47xx usb device */
#define	BCM47XX_IPSEC_ID	0x4718		/* 47xx ipsec */
#define	BCM47XX_ROBO_ID		0x4719		/* 47xx/53xx roboswitch core */
#define	BCM47XX_USB20H_ID	0x471a		/* 47xx usb 2.0 host */
#define	BCM47XX_USB20D_ID	0x471b		/* 47xx usb 2.0 device */
#define	BCM47XX_ATA100_ID	0x471d		/* 47xx parallel ATA */
#define	BCM47XX_SATAXOR_ID	0x471e		/* 47xx serial ATA & XOR DMA */
#define	BCM47XX_GIGETH_ID	0x471f		/* 47xx GbE (5700) */
#define	BCM4712_MIPS_ID		0x4720		/* 4712 base devid */
#define	BCM4716_DEVICE_ID	0x4722		/* 4716 base devid */
#define BCM47XX_SMBUS_EMU_ID	0x47fe		/* 47xx emulated SMBus device */
#define	BCM47XX_XOR_EMU_ID	0x47ff		/* 47xx emulated XOR engine */
#define	EPI41210_DEVICE_ID	0xa0fa		/* bcm4210 */
#define	EPI41230_DEVICE_ID	0xa10e		/* bcm4230 */
#define JINVANI_SDIOH_ID	0x4743		/* Jinvani SDIO Gold Host */
#endif /* !defined(ESTA_POSTMOGRIFY_REMOVAL) */
#define BCM27XX_SDIOH_ID	0x2702		/* BCM27xx Standard SDIO Host */
#define PCIXX21_FLASHMEDIA0_ID	0x8033		/* TI PCI xx21 Standard Host Controller */
#define PCIXX21_SDIOH0_ID	0x8034		/* TI PCI xx21 Standard Host Controller */
#define PCIXX21_FLASHMEDIA_ID	0x803b		/* TI PCI xx21 Standard Host Controller */
#define PCIXX21_SDIOH_ID	0x803c		/* TI PCI xx21 Standard Host Controller */
#define R5C822_SDIOH_ID		0x0822		/* Ricoh Co Ltd R5C822 SD/SDIO/MMC/MS/MSPro Host */
#define JMICRON_SDIOH_ID	0x2381		/* JMicron Standard SDIO Host Controller */

/* Chip IDs */
#define	BCM4306_CHIP_ID		0x4306		/* 4306 chipcommon chipid */
#define	BCM4311_CHIP_ID		0x4311		/* 4311 PCIe 802.11a/b/g */
#define	BCM4312_CHIP_ID		0x4312		/* 4312 chipcommon chipid */
#define	BCM4315_CHIP_ID		0x4315		/* 4315 chip id */
#define	BCM4318_CHIP_ID		0x4318		/* 4318 chipcommon chipid */
#define	BCM4319_CHIP_ID		0x4319		/* 4319 chipcommon chipid */
#define	BCM4320_CHIP_ID		0x4320		/* 4320 chipcommon chipid */
#define	BCM4321_CHIP_ID		0x4321		/* 4321 chipcommon chipid */
#define	BCM4322_CHIP_ID		0x4322		/* 4322 chipcommon chipid */
#define	BCM4325_CHIP_ID		0x4325		/* 4325 chip id */
#define	BCM4328_CHIP_ID		0x4328		/* 4328 chip id */
#define	BCM4329_CHIP_ID		0x4329		/* 4329 chipcommon chipid */
#define BCM4336_CHIP_ID		0x4336		/* 4336 chipcommon chipid */
#define	BCM4402_CHIP_ID		0x4402		/* 4402 chipid */
#define	BCM4704_CHIP_ID		0x4704		/* 4704 chipcommon chipid */
#define	BCM4710_CHIP_ID		0x4710		/* 4710 chipid */
#define	BCM4712_CHIP_ID		0x4712		/* 4712 chipcommon chipid */
#define BCM4785_CHIP_ID		0x4785		/* 4785 chipcommon chipid */
#define	BCM5350_CHIP_ID		0x5350		/* 5350 chipcommon chipid */
#define	BCM5352_CHIP_ID		0x5352		/* 5352 chipcommon chipid */
#define	BCM5354_CHIP_ID		0x5354		/* 5354 chipcommon chipid */
#define BCM5365_CHIP_ID		0x5365          /* 5365 chipcommon chipid */


/* Package IDs */
#define	BCM4303_PKG_ID		2		/* 4303 package id */
#define	BCM4309_PKG_ID		1		/* 4309 package id */
#define	BCM4712LARGE_PKG_ID	0		/* 340pin 4712 package id */
#define	BCM4712SMALL_PKG_ID	1		/* 200pin 4712 package id */
#define	BCM4712MID_PKG_ID	2		/* 225pin 4712 package id */
#define BCM4328USBD11G_PKG_ID	2		/* 4328 802.11g USB package id */
#define BCM4328USBDUAL_PKG_ID	3		/* 4328 802.11a/g USB package id */
#define BCM4328SDIOD11G_PKG_ID	4		/* 4328 802.11g SDIO package id */
#define BCM4328SDIODUAL_PKG_ID	5		/* 4328 802.11a/g SDIO package id */
#define BCM4329_289PIN_PKG_ID	0		/* 4329 289-pin package id */
#define BCM4329_182PIN_PKG_ID	1		/* 4329N 182-pin package id */
#define BCM5354E_PKG_ID		1		/* 5354E package id */
#define HDLSIM5350_PKG_ID	1		/* HDL simulator package id for a 5350 */
#define HDLSIM_PKG_ID		14		/* HDL simulator package id */
#define HWSIM_PKG_ID		15		/* Hardware simulator package id */

#if !defined(ESTA_POSTMOGRIFY_REMOVAL)
/* boardflags */
#define	BFL_BTCOEXIST	0x00000001	/* This board implements Bluetooth coexistence */
#define	BFL_PACTRL	0x00000002	/* This board has gpio 9 controlling the PA */
#define	BFL_AIRLINEMODE	0x00000004	/* This board implements gpio13 radio disable indication */
#define	BFL_ADCDIV	0x00000008	/* This board has the rssi ADC divider */
#define	BFL_ENETROBO	0x00000010	/* This board has robo switch or core */
#define	BFL_NOPLLDOWN	0x00000020	/* Not ok to power down the chip pll and oscillator */
#define	BFL_CCKHIPWR	0x00000040	/* Can do high-power CCK transmission */
#define	BFL_ENETADM	0x00000080	/* This board has ADMtek switch */
#define	BFL_ENETVLAN	0x00000100	/* This board has vlan capability */
#ifdef WLAFTERBURNER
#define	BFL_AFTERBURNER	0x00000200	/* This board supports Afterburner mode */
#endif /* WLAFTERBURNER */
#define BFL_NOPCI	0x00000400	/* This board leaves PCI floating */
#define BFL_FEM		0x00000800	/* This board supports the Front End Module */
#define BFL_EXTLNA	0x00001000	/* This board has an external LNA for 2.4GHz */
#define BFL_HGPA	0x00002000	/* This board has a high gain PA */
#define	BFL_BTCMOD	0x00004000	/* This board' BTCOEXIST is in the alternate gpios */
#define	BFL_ALTIQ	0x00008000	/* Alternate I/Q settings */
#define BFL_NOPA	0x00010000	/* This board has no PA */
#define BFL_RSSIINV	0x00020000	/* This board's RSSI uses positive slope(not TSSI) */
#define BFL_PAREF	0x00040000	/* This board uses the PARef LDO */
#define BFL_3TSWITCH	0x00080000	/* This board uses a triple throw switch shared with BT */
#define BFL_PHASESHIFT	0x00100000	/* This board can support phase shifter */
#define BFL_BUCKBOOST	0x00200000	/* Power topology uses BUCKBOOST */
#define BFL_FEM_BT	0x00400000	/* This board has FEM and switch to share antenna w/ BT */
#define BFL_NOCBUCK	0x00800000	/* Power topology doesn't use CBUCK */
#define BFL_CCKFAVOREVM	0x01000000	/* Favor CCK EVM over spectral mask */
#define BFL_PALDO	0x02000000	/* Power topology uses PALDO */
#define BFL_LNLDO2_2P5	0x04000000	/* Select 2.5V as LNLDO2 output voltage */
#define BFL_FASTPWR	0x08000000
#define BFL_EXTLNA_5GHz	0x10000000	/* This board has an external LNA for 5GHz */
#define BFL_EXTLNA_TX	0x20000000	/* Temp boardflag to indicate to */
					/* ucode control of eLNA during Tx */


/* boardflags2 */
#define BFL2_RXBB_INT_REG_DIS	0x00000001	/* This board has an external rxbb regulator */
#define BFL2_DEPRECIATED_STUB	0x00000002	/* This board flag is depreciated */
#define BFL2_TXPWRCTRL_EN	0x00000004	/* This board permits enabling TX Power Control */
#define BFL2_2X4_DIV		0x00000008	/* This board supports the 2X4 diversity switch */
#define BFL2_5G_PWRGAIN		0x00000010	/* This board supports 5G band power gain */
#define BFL2_PCIEWAR_OVR	0x00000020	/* This board overrides ASPM and Clkreq settings */
#define BFL2_CAESERS_BRD	0x00000040	/* This board is Caesers brd (unused by sw) */
#define BFL2_BTC3WIRE		0x00000080	/* This board used 3-wire BTC */
#define BFL2_SKWRKFEM_BRD	0x00000100	/* This board 4321mcm93 uses Skyworks FEM */
#define BFL2_ELNACTRL_TRSW_2G   0x00400000      /* For Some AZW 4329 Designs 2G */
						/* gmode_elna_gain conrols TR Switch */

/* board specific GPIO assignment, gpio 0-3 are also customer-configurable led */
#define	BOARD_GPIO_BTC3W_IN	0x850	/* bit 4 is RF_ACTIVE, bit 6 is STATUS, bit 11 is PRI */
#define	BOARD_GPIO_BTC3W_OUT	0x020	/* bit 5 is TX_CONF */
#define	BOARD_GPIO_BTCMOD_IN	0x010	/* bit 4 is the alternate BT Coexistence Input */
#define	BOARD_GPIO_BTCMOD_OUT	0x020	/* bit 5 is the alternate BT Coexistence Out */
#define	BOARD_GPIO_BTC_IN	0x080	/* bit 7 is BT Coexistence Input */
#define	BOARD_GPIO_BTC_OUT	0x100	/* bit 8 is BT Coexistence Out */
#define	BOARD_GPIO_PACTRL	0x200	/* bit 9 controls the PA on new 4306 boards */
#define BOARD_GPIO_3		0x0008	/* gpio 3 */
#define BOARD_GPIO_12		0x1000	/* gpio 12 */
#define BOARD_GPIO_13		0x2000	/* gpio 13 */

#define	PCI_CFG_GPIO_SCS	0x10	/* PCI config space bit 4 for 4306c0 slow clock source */
#define PCI_CFG_GPIO_HWRAD	0x20	/* PCI config space GPIO 13 for hw radio disable */
#define PCI_CFG_GPIO_XTAL	0x40	/* PCI config space GPIO 14 for Xtal powerup */
#define PCI_CFG_GPIO_PLL	0x80	/* PCI config space GPIO 15 for PLL powerdown */

/* power control defines */
#define PLL_DELAY		150		/* us pll on delay */
#define FREF_DELAY		200		/* us fref change delay */
#define MIN_SLOW_CLK		32		/* us Slow clock period */
#define	XTAL_ON_DELAY		1000		/* us crystal power-on delay */

/* Reference Board Types */
#define	BU4710_BOARD		0x0400
#define	VSIM4710_BOARD		0x0401
#define	QT4710_BOARD		0x0402

#define	BU4309_BOARD		0x040a
#define	BCM94309CB_BOARD	0x040b
#define	BCM94309MP_BOARD	0x040c
#define	BCM4309AP_BOARD		0x040d

#define	BCM94302MP_BOARD	0x040e

#define	BU4306_BOARD		0x0416
#define	BCM94306CB_BOARD	0x0417
#define	BCM94306MP_BOARD	0x0418

#define	BCM94710D_BOARD		0x041a
#define	BCM94710R1_BOARD	0x041b
#define	BCM94710R4_BOARD	0x041c
#define	BCM94710AP_BOARD	0x041d

#define	BU2050_BOARD		0x041f

#define	BCM94306P50_BOARD	0x0420

#define	BCM94309G_BOARD		0x0421

#define	BU4704_BOARD		0x0423
#define	BU4702_BOARD		0x0424

#define	BCM94306PC_BOARD	0x0425		/* pcmcia 3.3v 4306 card */

#define	MPSG4306_BOARD		0x0427

#define	BCM94702MN_BOARD	0x0428

/* BCM4702 1U CompactPCI Board */
#define	BCM94702CPCI_BOARD	0x0429

/* BCM4702 with BCM95380 VLAN Router */
#define	BCM95380RR_BOARD	0x042a

/* cb4306 with SiGe PA */
#define	BCM94306CBSG_BOARD	0x042b

/* cb4306 with SiGe PA */
#define	PCSG94306_BOARD		0x042d

/* bu4704 with sdram */
#define	BU4704SD_BOARD		0x042e

/* Dual 11a/11g Router */
#define	BCM94704AGR_BOARD	0x042f

/* 11a-only minipci */
#define	BCM94308MP_BOARD	0x0430

/* 4306/gprs combo */
#define	BCM94306GPRS_BOARD	0x0432

/* BCM5365/BCM4704 FPGA Bringup Board */
#define BU5365_FPGA_BOARD	0x0433

#define BU4712_BOARD		0x0444
#define	BU4712SD_BOARD		0x045d
#define	BU4712L_BOARD		0x045f

/* BCM4712 boards */
#define BCM94712AP_BOARD	0x0445
#define BCM94712P_BOARD		0x0446

/* BCM4318 boards */
#define BU4318_BOARD		0x0447
#define CB4318_BOARD		0x0448
#define MPG4318_BOARD		0x0449
#define MP4318_BOARD		0x044a
#define SD4318_BOARD		0x044b

/* BCM63XX boards */
#define BCM96338_BOARD		0x6338
#define BCM96348_BOARD		0x6348
#define BCM96358_BOARD		0x6358
#define BCM96368_BOARD		0x6368

/* Another mp4306 with SiGe */
#define	BCM94306P_BOARD		0x044c

/* mp4303 */
#define	BCM94303MP_BOARD	0x044e

/* mpsgh4306 */
#define	BCM94306MPSGH_BOARD	0x044f

/* BRCM 4306 w/ Front End Modules */
#define BCM94306MPM  		0x0450
#define BCM94306MPL  		0x0453

/* 4712agr */
#define	BCM94712AGR_BOARD	0x0451

/* pcmcia 4303 */
#define	PC4303_BOARD		0x0454

/* 5350K */
#define	BCM95350K_BOARD		0x0455

/* 5350R */
#define	BCM95350R_BOARD		0x0456

/* 4306mplna */
#define	BCM94306MPLNA_BOARD	0x0457

/* 4320 boards */
#define	BU4320_BOARD		0x0458
#define	BU4320S_BOARD		0x0459
#define	BCM94320PH_BOARD	0x045a

/* 4306mph */
#define	BCM94306MPH_BOARD	0x045b

/* 4306pciv */
#define	BCM94306PCIV_BOARD	0x045c

#define	BU4712SD_BOARD		0x045d

#define	BCM94320PFLSH_BOARD	0x045e

#define	BU4712L_BOARD		0x045f
#define	BCM94712LGR_BOARD	0x0460
#define	BCM94320R_BOARD		0x0461

#define	BU5352_BOARD		0x0462

#define	BCM94318MPGH_BOARD	0x0463

#define	BU4311_BOARD		0x0464
#define	BCM94311MC_BOARD	0x0465
#define	BCM94311MCAG_BOARD	0x0466

#define	BCM95352GR_BOARD	0x0467

/* bcm95351agr */
#define	BCM95351AGR_BOARD	0x0470

/* bcm94704mpcb */
#define	BCM94704MPCB_BOARD	0x0472

/* 4785 boards */
#define BU4785_BOARD		0x0478

/* 4321 boards */
#define BU4321_BOARD		0x046b
#define BU4321E_BOARD		0x047c
#define MP4321_BOARD		0x046c
#define CB2_4321_BOARD		0x046d
#define CB2_4321_AG_BOARD	0x0066
#define MC4321_BOARD		0x046e

/* 4328 boards */
#define BU4328_BOARD		0x0481
#define BCM4328SDG_BOARD	0x0482
#define BCM4328SDAG_BOARD	0x0483
#define BCM4328UG_BOARD		0x0484
#define BCM4328UAG_BOARD	0x0485
#define BCM4328PC_BOARD		0x0486
#define BCM4328CF_BOARD		0x0487

/* 4325 boards */
#define BCM94325DEVBU_BOARD	0x0490
#define BCM94325BGABU_BOARD	0x0491

#define BCM94325SDGWB_BOARD	0x0492

#define BCM94325SDGMDL_BOARD    0x04aa
#define BCM94325SDGMDL2_BOARD	0x04c6
#define BCM94325SDGMDL3_BOARD	0x04c9

#define BCM94325SDABGWBA_BOARD  0x04e1

/* 4322 boards */
#define BCM94322MC_SSID		0x04a4
#define BCM94322U_SSID		0x04a8
#define BCM94322HM_SSID		0x04b0
#define BCM94322USB_SSID	0x04a8
#define BCM94322USB2D_SSID	0x04bf

/* 4315 boards */
#define BCM94315DEVBU_SSID	0x04c2
#define BCM94315USBGP_SSID  0x04c7
#define BCM94315BGABU_SSID  0x04ca
#define BCM94315USBGP41_SSID 0x04cb

/* 4319 boards */
#define BCM94319DEVBU_SSID	0X04e5
#define BCM94319USBNP4L_SSID	0X04e6
#define BCM94319WLUSBN4L_SSID	0X04e7
#define BCM94319SDG_SSID	0X04ea
#define BCM94319LCUSBSDN4L_SSID	0X04eb
#define BCM94319LCSDN4L_SSID	0X0507
#define BCM94319LSUSBN4L_SSID	0X0508
#define BCM94319SDNA4L_SSID	0X0517
#define BCM94319SDELNA4L_SSID	0X0518
#define BCM94319SDELNA6L_SSID	0X0539
#define BCM94319WINDSOR_SSID    0x0561
#define BCM94319MLAP_SSID       0x0562
#define BCM94319BHEMU3_SSID     0x0563
#define BCM94319ARCADYAN_SSID	0X0546


/* # of GPIO pins */
#define GPIO_NUMPINS		16

/* radio ID codes */
#define	NORADIO_ID		0xe4f5
#define	NORADIO_IDCODE		0x4e4f5246

#define	BCM2050_ID		0x2050
#define	BCM2050_IDCODE		0x02050000
#define	BCM2050A0_IDCODE	0x1205017f
#define	BCM2050A1_IDCODE	0x2205017f
#define	BCM2050R8_IDCODE	0x8205017f

#define BCM2055_ID		0x2055
#define BCM2055_IDCODE		0x02055000
#define BCM2055A0_IDCODE	0x1205517f

#define BCM2056_ID		0x2056
#define BCM2056_IDCODE		0x02056000
#define BCM2056A0_IDCODE	0x1205517f

#define	BCM2060_ID		0x2060
#define	BCM2060_IDCODE		0x02060000
#define	BCM2060WW_IDCODE	0x1206017f

#define BCM2062_ID		0x2062
#define BCM2062_IDCODE		0x02062000
#define BCM2062A0_IDCODE	0x0206217f

#define BCM2063_ID		0x2063
#define BCM2063_IDCODE		0x02063000
#define BCM2063A0_IDCODE	0x0206317f

/* parts of an idcode: */
#define	IDCODE_MFG_MASK		0x00000fff
#define	IDCODE_MFG_SHIFT	0
#define	IDCODE_ID_MASK		0x0ffff000
#define	IDCODE_ID_SHIFT		12
#define	IDCODE_REV_MASK		0xf0000000
#define	IDCODE_REV_SHIFT	28
#endif /* !defined(ESTA_POSTMOGRIFY_REMOVAL) */

#endif /* _BCMDEVS_H */
