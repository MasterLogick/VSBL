#!/bin/bash
BASEDIR=$(pwd)
vboxmanage storageattach Test_VM_64 --storagectl SATA --port 1 --medium none
vboxmanage closemedium $BASEDIR/Test_VM_64/Test_VM_64.vdi --delete
vboxmanage convertfromraw $BASEDIR/out/boot.img $BASEDIR/Test_VM_64/Test_VM_64.vdi --format VDI
vboxmanage storageattach Test_VM_64 --storagectl SATA --port 1 --type hdd --medium $BASEDIR/Test_VM_64/Test_VM_64.vdi
vboxmanage startvm Test_VM_64
