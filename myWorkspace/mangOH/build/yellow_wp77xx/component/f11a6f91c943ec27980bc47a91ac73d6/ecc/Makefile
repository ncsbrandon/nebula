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
#    Hauke Mehrtens - memory optimization, ECC integration
#

# the library's version
VERSION:=0.8.6

# tools

SHELL = /bin/sh
MKDIR = mkdir
CC=/home/firstuser/nebula/myWorkspace/leaf-data/MANGOH-YELLOW-WP77XX/mangOH-yellow-wp77xx-toolchain/sysroots/x86_64-pokysdk-linux/usr/bin/arm-poky-linux-gnueabi/arm-poky-linux-gnueabi-gcc

abs_builddir = /home/firstuser/nebula/myWorkspace/mangOH/build/yellow_wp77xx/component/f11a6f91c943ec27980bc47a91ac73d6/ecc
top_builddir = ..
top_srcdir:= ..

ECC_SOURCES:= ecc.c testecc.c testfield.c test_helper.c
ECC_HEADERS:= ecc.h test_helper.h
FILES:=Makefile.in Makefile.contiki $(ECC_SOURCES) $(ECC_HEADERS) 
DISTDIR=$(top_builddir)/tinydtls-0.8.6

ifeq ("@WITH_CONTIKI@", "1")
include Makefile.contiki
else
ECC_OBJECTS:= $(patsubst %.c, %.o, $(ECC_SOURCES)) ecc_test.o
PROGRAMS:= testecc testfield
CPPFLAGS=--sysroot=/home/firstuser/nebula/myWorkspace/leaf-data/MANGOH-YELLOW-WP77XX/mangOH-yellow-wp77xx-toolchain/sysroots/armv7a-neon-poky-linux-gnueabi  -DDTLSv12 -DWITH_SHA256
CFLAGS=-Wall -std=c99 -pedantic -w -std=gnu99 -DSIERRA -DWITH_SHA256 -I/home/firstuser/nebula/myWorkspace/leaf-data/MANGOH-YELLOW-WP77XX/mangOH-yellow-wp77xx-legato/3rdParty/Lwm2mCore/include/platform-specific/linux -fPIC --sysroot=/home/firstuser/nebula/myWorkspace/leaf-data/MANGOH-YELLOW-WP77XX/mangOH-yellow-wp77xx-toolchain/sysroots/armv7a-neon-poky-linux-gnueabi -O2 -DSIERRA_MDM9X28 -DSIERRA_WP76XX -DPCM_IF=SEC_AUX -DI2S_IF=SEC_MI2S -DUSB_IF=AFE_PCM -DCODEC_IF=PRI_MI2S -DMM_1_ALSA_MIXER_CTRL=MultiMedia1 -DMM_1_ALSA_PCM_DEVICE_ID=0 -DMM_2_ALSA_MIXER_CTRL=MultiMedia2 -DMM_2_ALSA_PCM_DEVICE_ID=13 -DADC_BEGIN_NUM=0 -DADC_COUNT=4 -DPDP_MAX_PROFILE=16 -DPDP_MAX_MULTIPDP=8 -DEUICC -DLWM2M_OBJECT_3_BATTERY -DLWM2M_OBJECT_9 -I /home/firstuser/nebula/myWorkspace/mangOH/build/yellow_wp77xx -I/home/firstuser/nebula/myWorkspace/leaf-data/MANGOH-YELLOW-WP77XX/mangOH-yellow-wp77xx-legato/interfaces -I/home/firstuser/nebula/myWorkspace/leaf-data/MANGOH-YELLOW-WP77XX/mangOH-yellow-wp77xx-legato/framework/include -I/home/firstuser/nebula/myWorkspace/leaf-data/MANGOH-YELLOW-WP77XX/mangOH-yellow-wp77xx-legato/build/wp77xx/framework/include -I. -I/home/firstuser/nebula/myWorkspace/leaf-data/MANGOH-YELLOW-WP77XX/mangOH-yellow-wp77xx-legato/interfaces/modemServices -I/home/firstuser/nebula/myWorkspace/leaf-data/MANGOH-YELLOW-WP77XX/mangOH-yellow-wp77xx-legato/interfaces/positioning -I/home/firstuser/nebula/myWorkspace/leaf-data/MANGOH-YELLOW-WP77XX/mangOH-yellow-wp77xx-legato/interfaces/atServices -I/home/firstuser/nebula/myWorkspace/mangOH/interfaces -I/home/firstuser/nebula/myWorkspace/mangOH/apps/LedService -I/home/firstuser/nebula/myWorkspace/mangOH/apps/DataHub -I/home/firstuser/nebula/myWorkspace/mangOH/apps/BatteryService -I/home/firstuser/nebula/myWorkspace/mangOH/apps/Bme680EnvironmentalSensor -I/home/firstuser/nebula/myWorkspace/mangOH/apps/YellowSensor/interfaces -I/home/firstuser/nebula/myWorkspace/leaf-data/MANGOH-YELLOW-WP77XX/mangOH-yellow-wp77xx-legato/interfaces/wifi -DTEST_INCLUDE
LDLIBS=

.PHONY: all dirs clean install distclean .gitignore doc

.SUFFIXES:
.SUFFIXES:      .c .o

all: $(PROGRAMS)

ecc_test.o:	ecc.c ecc.h
	$(CC) $(CFLAGS) $(CPPFLAGS)  -c -o $@ $<

testecc: ecc_test.o test_helper.o

testfield: ecc_test.o test_helper.o

check:	
	echo DISTDIR: $(DISTDIR)
	echo top_builddir: $(top_builddir)

clean:
	@rm -f $(PROGRAMS) main.o $(LIB) $(ECC_OBJECTS)
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir clean ; \
	done

distclean:	clean
	@rm -rf $(DISTDIR)
	@rm -f *~ $(DISTDIR).tar.gz
endif # WITH_CONTIKI

dist:	$(FILES)
	test -d $(DISTDIR)/ecc || mkdir $(DISTDIR)/ecc
	cp -p $(FILES) $(DISTDIR)/ecc

install:	$(ECC_HEADERS)
	test -d $(includedir)/ecc || mkdir -p $(includedir)/ecc
	$(install) $(ECC_HEADERS) $(includedir)/ecc

.gitignore:
	echo "core\n*~\n*.[oa]\n*.gz\n*.cap\n$(PROGRAM)\n$(DISTDIR)\n.gitignore" >$@
