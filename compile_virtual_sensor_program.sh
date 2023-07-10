#!/bin/bash

g++ main_csv.cpp ./lib/virtual_sensor/virtual_sensor.cpp -o virtual_sensor_program_csv
g++ main_udp.cpp ./lib/virtual_sensor/virtual_sensor.cpp -o virtual_sensor_program_udp
# Check if compilation was successful
if [ $? -eq 0 ]; then
  echo "Compilation successful. Program created: virtual_sensor_program"
else
  echo "Compilation failed"
fi

