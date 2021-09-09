#!/bin/bash
IMAGE_NAME="boot.img"
BASEDIR=$(pwd)
MACHINE_NAME="Test_VM"
vboxmanage storageattach $MACHINE_NAME --storagectl SATA --port 1 --medium none
vboxmanage closemedium $BASEDIR/$MACHINE_NAME/$MACHINE_NAME.vdi --delete
vboxmanage convertfromraw $BASEDIR/out/$IMAGE_NAME $BASEDIR/$MACHINE_NAME/$MACHINE_NAME.vdi --format VDI
vboxmanage storageattach $MACHINE_NAME --storagectl SATA --port 1 --type hdd --medium $BASEDIR/$MACHINE_NAME/$MACHINE_NAME.vdi