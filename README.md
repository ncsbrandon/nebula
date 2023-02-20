# nebula

## environment setup
mkdir ~/myWorkspace
cd ~/myWorkspace
leaf setup -p mangOH-yellow-wp77xx_0.7.0
git clone --recursive https://github.com/mangOH/mangOH
cd mangOH/components/boschBsec
wget https://community.bosch-sensortec.com/varuj77995/attachments/varuj77995/bst_community-mems-forum/44/1/BSEC_1.4.7.2_GCC_CortexA7_20190225.zip
unzip BSEC_1.4.7.2_GCC_CortexA7_20190225.zip
cd ../..
leaf shell
make yellow

## flashing the app
update build/update_files/yellow.wp77xx.update 192.168.2.2

## reading logs
logread -f | grep vegas

## app control
app start vegasMode
app stop vegasMode

## to completely reflash the device
scp Downloads/mangOH-yellow-wp77xx_0.7.0-octave.spk root@192.168.2.2:/tmp/

## factory reset
legato stop
rm -rf /mnt/flash/legato/*
reboot

## resources
I/O		NAME						TYPE		DEFAULT		DESC
--------------------------------------------------------------------
Output	flasher/enable				boolean		false		Should the flasher be used
Output	flasher/value				boolean		false		State of the digital output
Output	door/enable					boolean		false		Should the door monitor be used
Input	door/value					boolean					Is the door open (true) or closed (false)
Output	ac/enable					boolean		false		Should AC voltage be monitored
Input	ac/value					number					AC voltage
Output	solar/enable				boolean		false		Should solar voltage be monitored
Input	solar/value					number					DC voltage
Output	battery/enable				boolean		false		Should battery voltage be monitored
Input	battery/value				number					DC voltage