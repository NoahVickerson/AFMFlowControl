#!/bin/bash

. ~/esp32/esp-idf/export.sh
# idf.py fullclean
# idf.py reconfigure
idf.py build
idf.py flash

idf.py monitor
# screen /dev/tty.usbserial-110 115200
