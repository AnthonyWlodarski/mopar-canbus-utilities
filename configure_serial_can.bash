#!/bin/bash
#
# This script is intended for use with LAWICEL CANUSB hardware. During development
# the hardware in used was purchased from https://www.can232.com/?page_id=75
# 

# check to see if we 
if [ "$#" -ne 2 ]; then
	printf "Usage: $0 /dev/USBDEVICE CANNAME\n"
	printf "/dev/USBDEVICE is the actual hardware device, use lsusb or usb-devices to find.\n"
	printf "CANNAME is the name of the device you want. e.g. 'slcan0'\n"
	exit 1
fi

printf "WARINING: can-utils package must be installed!\n"
printf "********MODPROBE CHECK********\n"
sudo lsmod | grep can
printf "*Should have seen some can devices above.\n"


printf "\nSetting link to down if it exists.\n"
sudo ip link set down $2
printf "\nKilling the slcand process.\n"
sudo pkill slcand
printf "\nCreating can device with slcand.\n"
sudo slcand -o -s6 -t hw -S 3000000 $1 $2
printf "\nTaking a nap.\n"
sleep 2
printf "\nBringing link up.\n"
sudo ip link set up $2
sudo ip link show $2
printf "\nDevice $2 is ready for use.\n"
