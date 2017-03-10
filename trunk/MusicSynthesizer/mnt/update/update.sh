#!/bin/sh
#function change ip
#see ipconfig.ini
#call ipcfgdo.sh

flash_eraseall /dev/mtd7
dd if=/mnt/update/Dspupdate.dat of=/dev/mtd7