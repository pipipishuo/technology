echo remove boot directory
cd /boot/
ls |grep -E "*6.9.0*"| xargs rm
echo remove lib
cd /lib/modules/
ls |grep -E "*6.9.0*"| xargs rm -r
