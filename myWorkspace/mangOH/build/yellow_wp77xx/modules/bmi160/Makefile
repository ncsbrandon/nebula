obj-m += bmi160.o
bmi160-objs += bmi160_core.o

src = /home/firstuser/nebula/myWorkspace/mangOH/linux_kernel_modules/bmi160

ccflags-y += -DCONFIG_BMI160
ccflags-y += -DCONFIG_BMI160_I2C
ccflags-y += -DCONFIG_IIO
ccflags-y += -DCONFIG_IIO_BUFFER
ccflags-y += -DCONFIG_IIO_TRIGGERED_BUFFER
ccflags-y += -DREGMAP

KBUILD := /home/firstuser/nebula/myWorkspace/leaf-data/MANGOH-YELLOW-WP77XX/mangOH-yellow-wp77xx-toolchain/sysroots/armv7a-neon-poky-linux-gnueabi/usr/src/kernel
export CROSS_COMPILE := /home/firstuser/nebula/myWorkspace/leaf-data/MANGOH-YELLOW-WP77XX/mangOH-yellow-wp77xx-toolchain/sysroots/x86_64-pokysdk-linux/usr/bin/arm-poky-linux-gnueabi/arm-poky-linux-gnueabi-
export ARCH := arm

all:
	make -C $(KBUILD) M=/home/firstuser/nebula/myWorkspace/mangOH/build/yellow_wp77xx/modules/bmi160 modules

clean:
	 make -C $(KBUILD) M=/home/firstuser/nebula/myWorkspace/mangOH/build/yellow_wp77xx/modules/bmi160 clean
