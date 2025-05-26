#!/bin/bash

. ~/esp32/esp-idf/export.sh
# idf.py fullclean
# idf.py reconfigure
idf.py build
idf.py flash 2>&1 | tee "flash.txt"

match=$(grep -oE "/dev/cu\..*" "flash.txt" | tail -n 1)

echo $match

screen -L $match 115200 

rm -rf "humidity.csv"
rm -rf "pid.csv"

grep -a "hum: .*$" screenlog.0 | LC_ALL=C sed "s/.* (\(.*\)) .* humidity: \(.*\),.*/\1,\2,/g" > humidity.csv
grep -a "pid: .*$" screenlog.0 | LC_ALL=C sed "s/.* (\(.*\)) .* output: \(.*\),.*/\1,\2,/g" > pid.csv
 
rm -rf "flash.txt"
rm -rf "screenlog.0"
