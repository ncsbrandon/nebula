obj-m += bq27xxx_battery.o
bq27xxx_battery-objs += bq27xxx_battery_i2c.o
bq27xxx_battery-objs += devres_backport.o
bq27xxx_battery-objs += bq27xxx_battery_source.o
bq27xxx_battery-objs += power_supply_backport.o

src = /home/firstuser/nebula/myWorkspace/mangOH/linux_kernel_modules/bq27xxx


KBUILD := /home/firstuser/nebula/myWorkspace/leaf-data/MANGOH-YELLOW-WP77XX/mangOH-yellow-wp77xx-toolchain/sysroots/armv7a-neon-poky-linux-gnueabi/usr/src/kernel
export CROSS_COMPILE := /home/firstuser/nebula/myWorkspace/leaf-data/MANGOH-YELLOW-WP77XX/mangOH-yellow-wp77xx-toolchain/sysroots/x86_64-pokysdk-linux/usr/bin/arm-poky-linux-gnueabi/arm-poky-linux-gnueabi-
export ARCH := arm

all:
	make -C $(KBUILD) M=/home/firstuser/nebula/myWorkspace/mangOH/build/yellow_wp77xx/modules/bq27xxx_battery modules

clean:
	 make -C $(KBUILD) M=/home/firstuser/nebula/myWorkspace/mangOH/build/yellow_wp77xx/modules/bq27xxx_battery clean
