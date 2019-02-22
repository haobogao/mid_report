#!/bin/bash
DEV=`ls /dev/ttyACM*`
echo $DEV
result=$(echo $DEV | grep "$tty")
if [ "result" != "" ] ; then
make clean
make 
./mid_report.app $DEV
else
echo "serial not found!"
fi