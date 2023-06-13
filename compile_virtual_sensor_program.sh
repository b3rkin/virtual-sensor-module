#!/bin/bash

g++ main.cpp ./lib/virtual_sensor/virtual_sensor.cpp -o virtual_sensor_program

# Check if compilation was successful
if [ $? -eq 0 ]; then
  echo "Compilation successful. Program created: virtual_sensor_program"
else
  echo "Compilation failed"
fi

