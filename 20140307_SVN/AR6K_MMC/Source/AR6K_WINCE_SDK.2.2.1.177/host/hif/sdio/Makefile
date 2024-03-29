#------------------------------------------------------------------------------
# <copyright file="makefile" company="Atheros">
#    Copyright (c) 2005-2008 Atheros Corporation.  All rights reserved.
# 
# The software source and binaries included in this development package are
# licensed, not sold. You, or your company, received the package under one
# or more license agreements. The rights granted to you are specifically
# listed in these license agreement(s). All other rights remain with Atheros
# Communications, Inc., its subsidiaries, or the respective owner including
# those listed on the included copyright notices.  Distribution of any
# portion of this package must be in strict compliance with the license
# agreement(s) terms.
# </copyright>
# 
# <summary>
# 	Wifi driver for AR6002
# </summary>
#
#------------------------------------------------------------------------------
#==============================================================================
# Author(s): ="Atheros"
#==============================================================================

#
#SDIO HIF makefile for atheros SDIO stack
#  

# Check for SDIO stack
ifdef ATH_SDIO_STACK_BASE
# Someone already set it on entry, the stack resides outside this tree, we will try to build it
_SDIO_STACK = YES
else
# Check for SDIO stack within this tree
_SDIO_STACK = $(shell if [ -f $(ATH_SRC_BASE)/sdiostack/src/Makefile ]; then echo "YES"; else echo "NO"; fi)

ifeq ($(_SDIO_STACK), YES)
    # SDIO stack is part of the kit and will need to be compiled
ATH_SDIO_STACK_BASE := $(ATH_SRC_BASE)/sdiostack
endif
endif 


ifeq ($(ATH_BUS_SUBTYPE),linux_sdio)
_HIF_SUB_TYPE = linux_sdio
_SDIO_STACK = NO
else
_HIF_SUB_TYPE = linux_athsdio
endif


ifeq ($(_SDIO_STACK), YES)
    # Pass and translate build variables to the SDIO stack makefile
_SDIO_STACK_MAKE_PARAMS := CT_BUILD_TYPE=$(ATH_BUILD_TYPE) \
                           CT_OS_TYPE=linux \
                           CT_OS_SUB_TYPE=$(ATH_OS_SUB_TYPE) \
                           CT_LINUXPATH=$(ATH_LINUXPATH) \
                           CT_BUILD_TYPE=$(ATH_BUILD_TYPE) \
                           CT_CROSS_COMPILE_TYPE=$(ATH_CROSS_COMPILE_TYPE) \
                           CT_ARCH_CPU_TYPE=$(ATH_ARCH_CPU_TYPE) \
                           CT_HC_DRIVERS=$(ATH_HC_DRIVERS) \
                           CT_MAKE_INCLUDE_OVERRIDE=$(_LOCALMAKE_INCLUDE)   \
                           CT_BUILD_OUTPUT_OVERRIDE=$(COMPILED_IMAGE_OBJECTS_PATH) \
                           BUS_BUILD=1
endif
EXTRA_CFLAGS += -I$(ATH_SRC_BASE)/hif/sdio/$(_HIF_SUB_TYPE)/include
EXTRA_CFLAGS += -DSDIO
EXTRA_CFLAGS += -I$(ATH_SDIO_STACK_BASE)/src/include

ifeq ($(ATH_OS_SUB_TYPE),linux_2_4)
obj-y += ../../hif/sdio/linux_athsdio/src/hif.o
endif

ifneq ($(ATH_OS_SUB_TYPE),linux_2_4)
ar6000-objs := ../../hif/sdio/$(_HIF_SUB_TYPE)/src/hif.o
endif

all:
ifeq ($(_SDIO_STACK),YES)
	$(MAKE) $(_SDIO_STACK_MAKE_PARAMS) -C  $(ATH_SDIO_STACK_BASE)/src default
endif

clean:
ifeq ($(_SDIO_STACK),YES)
	$(MAKE) $(_SDIO_STACK_MAKE_PARAMS) -C $(ATH_SDIO_STACK_BASE)/src clean
endif
