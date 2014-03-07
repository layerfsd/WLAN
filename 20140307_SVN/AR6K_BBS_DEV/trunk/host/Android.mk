#------------------------------------------------------------------------------
# <copyright file="makefile" company="Atheros">
#    Copyright (c) 2005-2010 Atheros Corporation.  All rights reserved.
# 
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License version 2 as
# published by the Free Software Foundation;
#
# Software distributed under the License is distributed on an "AS
# IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or
# implied. See the License for the specific language governing
# rights and limitations under the License.
#
#
#------------------------------------------------------------------------------
#==============================================================================
# Author(s): ="Atheros"
#==============================================================================

ifneq ($(TARGET_SIMULATOR),true)

LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

ATH_ANDROID_SRC_BASE:= $(BOARD_WLAN_ATHEROS_SDK)
export  ATH_BUILD_TYPE=ANDROID_ARM_NATIVEMMC
export  ATH_BUS_TYPE=sdio
export  ATH_OS_SUB_TYPE=linux_2_6

ATH_ANDROID_ROOT:= $(CURDIR)
export ATH_SRC_BASE:=$(ATH_ANDROID_ROOT)/$(BOARD_WLAN_ATHEROS_SDK)/host
#ATH_CROSS_COMPILE_TYPE:=$(ATH_ANDROID_ROOT)/$(TARGET_TOOLS_PREFIX)
ATH_CROSS_COMPILE_TYPE:=$(ATH_ANDROID_ROOT)/prebuilt/linux-x86/toolchain/arm-eabi-4.3.1/bin/arm-eabi-
ATH_TARGET_OUTPUT:=$(ATH_ANDROID_ROOT)

ifeq ($(TARGET_PRODUCT),$(filter $(TARGET_PRODUCT),qsd8250_surf qsd8250_ffa msm7627_surf msm7627_ffa msm7625_ffa msm7625_surf msm7630_surf))
export  ATH_LINUXPATH=$(TARGET_OUT_INTERMEDIATES)/KERNEL_OBJ
else
# Comment out the following variable for your platform 
# Link your kernel into android SDK directory as 'kernel' directory
# export  ATH_LINUXPATH= [Your android/kernel path ]
endif 
export  ATH_ARCH_CPU_TYPE=arm
export  ATH_BUS_SUBTYPE=linux_sdio
export  ATH_ANDROID_ENV=yes
export  ATH_EEPROM_FILE_USED=no
export  ATH_SOFTMAC_USED=no

ATH_HIF_TYPE:=sdio

ifneq ($(PLATFORM_VERSION),$(filter $(PLATFORM_VERSION),1.5 1.6))

ifeq ($(TARGET_PRODUCT),$(filter $(TARGET_PRODUCT),msm7627_surf msm7627_ffa europa))
ATH_ANDROID_BUILD_FLAGS=-D__LINUX_ARM_ARCH__=6 -march=armv6 
#-fstack-check="generic"
endif

ifeq ($(TARGET_PRODUCT),$(filter $(TARGET_PRODUCT),qsd8250_surf qsd8250_ffa msm7630_surf smdkc100))
ATH_ANDROID_BUILD_FLAGS=-D__LINUX_ARM_ARCH__=7 -march=armv7-a 
#-fstack-check="generic"
endif

endif  # ECLAIR

#Uncomment the following define in order to enable OTA mode
#ATH_ANDROID_BUILD_FLAGS += -DATH6K_CONFIG_OTA_MODE

export ATH_ANDROID_BUILD_FLAGS

mod_cleanup := $(ATH_TARGET_OUTPUT)/$(ATH_ANDROID_SRC_BASE)/dummy

$(mod_cleanup) :
	rm -f `find $(ATH_TARGET_OUTPUT)/$(ATH_ANDROID_SRC_BASE) -name "*.o"`
	mkdir -p $(TARGET_OUT)/wifi
    
mod_file := os/linux/ar6000.ko
$(ATH_ANDROID_SRC_BASE)/host/$(mod_file) : $(mod_cleanup) $(TARGET_PREBUILT_KERNEL) $(ACP)
	$(MAKE) ARCH=arm CROSS_COMPILE=$(ATH_CROSS_COMPILE_TYPE) -C $(ATH_LINUXPATH) ATH_HIF_TYPE=$(ATH_HIF_TYPE) SUBDIRS=$(ATH_SRC_BASE)/os/linux modules
	$(ATH_CROSS_COMPILE_TYPE)strip -g -S -d $(ATH_ANDROID_SRC_BASE)/host/$(mod_file)
	$(ACP) -fpt $(ATH_ANDROID_SRC_BASE)/host/$(mod_file) $(TARGET_OUT)/wifi

LOCAL_MODULE := ar6000.ko
LOCAL_MODULE_TAGS := debug eng optional
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_PATH := $(TARGET_OUT)/wifi
LOCAL_SRC_FILES := $(mod_file)
include $(BUILD_PREBUILT)

include $(LOCAL_PATH)/tools/Android.mk

endif
