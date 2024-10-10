#!/bin/bash
qemu-system-x86_64 -enable-kvm -name ubuntutest  -m 4096 -hda ubuntutest.img -boot d -virtfs local,path=/home/pipishuo/KVM_Share/,mount_tag=hostshare,security_model=none,id=hostshare -s -S&
sleep 5
gdb -x gdbCommand

