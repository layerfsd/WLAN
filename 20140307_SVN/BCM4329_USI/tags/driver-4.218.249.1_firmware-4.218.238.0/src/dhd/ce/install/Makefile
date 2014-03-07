#
# Master makefile for producing all CE variants
#
# Copyright 1998, Epigram, Inc.
#
# $Id: Makefile,v 1.3 2007/06/21 23:41:53 Exp $
#

SRCBASE	        := ../../..
empty           :=
space           :=$(empty) $(empty)

TARGET_DEVNAMES := qphone bcm
TARGET_DEVTYPES := sdio
TARGET_PROCS    := X86 ARM
TARGET_OSVERS   := 500 600

VALID_TARGETS   := $(foreach devtype,$(TARGET_DEVTYPES), \
                   $(foreach devproc,$(TARGET_PROCS),    \
                   $(foreach devos,$(TARGET_OSVERS),     \
                   $(foreach devname,$(TARGET_DEVNAMES), \
                   ce$(devos)_$(devproc)_$(devtype)_$(devname)))))

TARGETS         ?=
TARGETS         += ce500_X86_sdio_qphone
TARGETS         += ce600_X86_sdio_qphone
TARGETS         += ce500_ARM_sdio_qphone
TARGETS         += ce600_ARM_sdio_qphone
TARGETS         += ce500_X86_sdio_bcm
TARGETS         += ce600_X86_sdio_bcm
TARGETS         += ce500_ARM_sdio_bcm
TARGETS         += ce600_ARM_sdio_bcm

all: $(TARGETS)

# Unlike CE/NIC mode driver installer, devtype is ignored.
# i.e. DEVTYPE is dummy variant, but kept here for consistency
$(TARGETS): DEVOS=$(subst ce,,$(word 1,$(subst _,$(space),$@)))
$(TARGETS): DEVPROC=$(word 2,$(subst _,$(space),$@))
$(TARGETS): DEVTYPE=$(word 3,$(subst _,$(space),$@))
$(TARGETS): DEVNAME=$(word 4,$(subst _,$(space),$@))
$(TARGETS):
	$(MAKE) -C $(DEVNAME) WINCEVER=$(DEVOS) PROCESSOR=$(DEVPROC) $(ACTION)

clean:
	$(MAKE) ACTION=clean $(TARGETS)

.PHONY: all clean
