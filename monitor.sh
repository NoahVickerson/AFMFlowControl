port=$(ls /dev/tty.* | grep -o "/dev/tty\.usbserial.*" | tail -n 1)
echo $port

screen -L $port 115200 

rm -rf "humidity.csv"
rm -rf "pid.csv"

grep -a "hum: .*$" screenlog.0 | LC_ALL=C sed "s/.* (\(.*\)) .* humidity: \(.*\),.*/\1,\2,/g" > humidity.csv
grep -a "pid: .*$" screenlog.0 | LC_ALL=C sed "s/.* (\(.*\)) .* output: \(.*\),.*/\1,\2,/g" > pid.csv
 
rm -rf "screenlog.0"
