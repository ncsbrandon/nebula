# Makefile for tinydtls
#
# Copyright (c) 2011, 2012, 2013, 2014, 2015 Olaf Bergmann (TZI) and others.
# All rights reserved. This program and the accompanying materials
# are made available under the terms of the Eclipse Public License v1.0
# and Eclipse Distribution License v. 1.0 which accompanies this distribution.
#
# The Eclipse Public License is available at http://www.eclipse.org/legal/epl-v10.html
# and the Eclipse Distribution License is available at 
# http://www.eclipse.org/org/documents/edl-v10.php.
#
# Contributors:
#    Olaf Bergmann  - initial API and implementation
#

# the library's version
VERSION:=0.8.6

# tools

SHELL = /bin/sh
MKDIR = mkdir
CC=/home/firstuser/nebula/myWorkspace/leaf-data/MANGOH-YELLOW-WP77XX/mangOH-yellow-wp77xx-toolchain/sysroots/x86_64-pokysdk-linux/usr/bin/arm-poky-linux-gnueabi/arm-poky-linux-gnueabi-gcc

abs_builddir = /home/firstuser/nebula/myWorkspace/mangOH/build/yellow_wp77xx/component/f11a6f91c943ec27980bc47a91ac73d6/aes
top_builddir = ..
top_srcdir:= ..

SOURCES:= rijndael.c
HEADERS:= rijndael.h
OBJECTS:= $(patsubst %.c, %.o, $(SOURCES))
CPPFLAGS=--sysroot=/home/firstuser/nebula/myWorkspace/leaf-data/MANGOH-YELLOW-WP77XX/mangOH-yellow-wp77xx-toolchain/sysroots/armv7a-neon-poky-linux-gnueabi  -DDTLSv12 -DWITH_SHA256
CFLAGS=-Wall -std=c99 -pedantic -w -std=gnu99 -DSIERRA -DWITH_SHA256 -I/home/firstuser/nebula/myWorkspace/leaf-data/MANGOH-YELLOW-WP77XX/mangOH-yellow-wp77xx-legato/3rdParty/Lwm2mCore/include/platform-specific/linux -fPIC --sysroot=/home/firstuser/nebula/myWorkspace/leaf-data/MANGOH-YELLOW-WP77XX/mangOH-yellow-wp77xx-toolchain/sysroots/armv7a-neon-poky-linux-gnueabi -O2 -DSIERRA_MDM9X28 -DSIERRA_WP76XX -DPCM_IF=SEC_AUX -DI2S_IF=SEC_MI2S -DUSB_IF=AFE_PCM -DCODEC_IF=PRI_MI2S -DMM_1_ALSA_MIXER_CTRL=MultiMedia1 -DMM_1_ALSA_PCM_DEVICE_ID=0 -DMM_2_ALSA_MIXER_CTRL=MultiMedia2 -DMM_2_ALSA_PCM_DEVICE_ID=13 -DADC_BEGIN_NUM=0 -DADC_COUNT=4 -DPDP_MAX_PROFILE=16 -DPDP_MAX_MULTIPDP=8 -DEUICC -DLWM2M_OBJECT_3_BATTERY -DLWM2M_OBJECT_9 -I /home/firstuser/nebula/myWorkspace/mangOH/build/yellow_wp77xx -I/home/firstuser/nebula/myWorkspace/leaf-data/MANGOH-YELLOW-WP77XX/mangOH-yellow-wp77xx-legato/interfaces -I/home/firstuser/nebula/myWorkspace/leaf-data/MANGOH-YELLOW-WP77XX/mangOH-yellow-wp77xx-legato/framework/include -I/home/firstuser/nebula/myWorkspace/leaf-data/MANGOH-YELLOW-WP77XX/mangOH-yellow-wp77xx-legato/build/wp77xx/framework/include -I. -I/home/firstuser/nebula/myWorkspace/leaf-data/MANGOH-YELLOW-WP77XX/mangOH-yellow-wp77xx-legato/interfaces/modemServices -I/home/firstuser/nebula/myWorkspace/leaf-data/MANGOH-YELLOW-WP77XX/mangOH-yellow-wp77xx-legato/interfaces/positioning -I/home/firstuser/nebula/myWorkspace/leaf-data/MANGOH-YELLOW-WP77XX/mangOH-yellow-wp77xx-legato/interfaces/atServices -I/home/firstuser/nebula/myWorkspace/mangOH/interfaces -I/home/firstuser/nebula/myWorkspace/mangOH/apps/LedService -I/home/firstuser/nebula/myWorkspace/mangOH/apps/DataHub -I/home/firstuser/nebula/myWorkspace/mangOH/apps/BatteryService -I/home/firstuser/nebula/myWorkspace/mangOH/apps/Bme680EnvironmentalSensor -I/home/firstuser/nebula/myWorkspace/mangOH/apps/YellowSensor/interfaces -I/home/firstuser/nebula/myWorkspace/leaf-data/MANGOH-YELLOW-WP77XX/mangOH-yellow-wp77xx-legato/interfaces/wifi
LDLIBS=
FILES:=Makefile.in $(SOURCES) $(HEADERS) 
DISTDIR=$(top_builddir)/tinydtls-0.8.6

.PHONY: all dirs clean install dist distclean .gitignore doc

.SUFFIXES:
.SUFFIXES:      .c .o

all:

check:	
	echo DISTDIR: $(DISTDIR)
	echo top_builddir: $(top_builddir)

clean:
	@rm -f $(PROGRAMS) main.o $(LIB) $(OBJECTS)
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir clean ; \
	done

distclean:	clean
	@rm -rf $(DISTDIR)
	@rm -f *~ $(DISTDIR).tar.gz

dist:	$(FILES)
	test -d $(DISTDIR)/aes || mkdir $(DISTDIR)/aes
	cp -p $(FILES) $(DISTDIR)/aes

install:	$(HEADERS)
	test -d $(includedir)/aes || mkdir -p $(includedir)/aes
	$(install) $(HEADERS) $(includedir)/aes

.gitignore:
	echo "core\n*~\n*.[oa]\n*.gz\n*.cap\n$(PROGRAM)\n$(DISTDIR)\n.gitignore" >$@
