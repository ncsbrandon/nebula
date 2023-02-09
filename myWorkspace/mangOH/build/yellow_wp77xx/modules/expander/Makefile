obj-m += expander.o

src = /home/firstuser/nebula/myWorkspace/mangOH/linux_kernel_modules/expander

ccflags-y += -DDEBUG

KBUILD := /home/firstuser/nebula/myWorkspace/leaf-data/MANGOH-YELLOW-WP77XX/mangOH-yellow-wp77xx-toolchain/sysroots/armv7a-neon-poky-linux-gnueabi/usr/src/kernel
export CROSS_COMPILE := /home/firstuser/nebula/myWorkspace/leaf-data/MANGOH-YELLOW-WP77XX/mangOH-yellow-wp77xx-toolchain/sysroots/x86_64-pokysdk-linux/usr/bin/arm-poky-linux-gnueabi/arm-poky-linux-gnueabi-
export ARCH := arm

all:
	make -C $(KBUILD) M=/home/firstuser/nebula/myWorkspace/mangOH/build/yellow_wp77xx/modules/expander modules

clean:
	 make -C $(KBUILD) M=/home/firstuser/nebula/myWorkspace/mangOH/build/yellow_wp77xx/modules/expander clean
