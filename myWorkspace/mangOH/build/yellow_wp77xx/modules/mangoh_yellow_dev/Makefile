obj-m += mangoh_yellow_dev.o
mangoh_yellow_dev-objs += mangoh_yellow.o

src = /home/firstuser/nebula/myWorkspace/mangOH/linux_kernel_modules/mangoh

ccflags-y += -DDEBUG
ccflags-y += -I/home/firstuser/nebula/myWorkspace/mangOH/linux_kernel_modules/expander
ccflags-y += -I/home/firstuser/nebula/myWorkspace/mangOH/linux_kernel_modules/opt300x
ccflags-y += -I/home/firstuser/nebula/myWorkspace/mangOH/linux_kernel_modules/bq27xxx
ccflags-y += -I/home/firstuser/nebula/myWorkspace/mangOH/linux_kernel_modules/bq25601

KBUILD := /home/firstuser/nebula/myWorkspace/leaf-data/MANGOH-YELLOW-WP77XX/mangOH-yellow-wp77xx-toolchain/sysroots/armv7a-neon-poky-linux-gnueabi/usr/src/kernel
export CROSS_COMPILE := /home/firstuser/nebula/myWorkspace/leaf-data/MANGOH-YELLOW-WP77XX/mangOH-yellow-wp77xx-toolchain/sysroots/x86_64-pokysdk-linux/usr/bin/arm-poky-linux-gnueabi/arm-poky-linux-gnueabi-
export ARCH := arm

all:
	make -C $(KBUILD) M=/home/firstuser/nebula/myWorkspace/mangOH/build/yellow_wp77xx/modules/mangoh_yellow_dev 'KBUILD_EXTRA_SYMBOLS=/home/firstuser/nebula/myWorkspace/mangOH/build/yellow_wp77xx/modules/bmi160-i2c/Module.symvers /home/firstuser/nebula/myWorkspace/mangOH/build/yellow_wp77xx/modules/bq25601/Module.symvers /home/firstuser/nebula/myWorkspace/mangOH/build/yellow_wp77xx/modules/bq27xxx_battery/Module.symvers /home/firstuser/nebula/myWorkspace/mangOH/build/yellow_wp77xx/modules/expander/Module.symvers /home/firstuser/nebula/myWorkspace/mangOH/build/yellow_wp77xx/modules/opt300x/Module.symvers /home/firstuser/nebula/myWorkspace/mangOH/build/yellow_wp77xx/modules/rtc-pcf85063/Module.symvers ' modules

clean:
	 make -C $(KBUILD) M=/home/firstuser/nebula/myWorkspace/mangOH/build/yellow_wp77xx/modules/mangoh_yellow_dev clean
