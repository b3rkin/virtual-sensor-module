# The Project:
This folder contains a c++ program that takes perfect (ground truth) position (x,y,z) and orientation (roll, pitch, yaw)  data and turns it into realistic accelerometer and gyroscope data.

## Current status of the project:
Reads 2 lines from input.csv writes the last read line to output.csv without any processing.

## I/O

| Input      | Output |
| ----------- | ----------- |
| A (.csv) file with orientation and position.     | A (.csv) file with accelerometer and gyroscope values.       |
| input.csv: x,y,z,r,p,y,TS  | output.csv: aX,aY,aZ,gX,gY,gZ,Ts |


## TO-DO
 - [ ] Add the conversion from tracking points (position + orientation) to sensor values.
 - [ ] Add realistic noise to the obtained sensor values.