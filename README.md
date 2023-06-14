# The Project:
This folder contains a c++ program that takes perfect (ground truth) position (x,y,z) and orientation (roll, pitch, yaw)  data and turns it into realistic accelerometer and gyroscope data.

## Current status of the project:
Reads 2 lines from input.csv writes the last read line to output.csv without any processing.

## I/O

| Input      | Output |
| ----------- | ----------- |
| A (.csv) file with orientation and position.     | A (.csv) file with accelerometer and gyroscope values.       |
| input.csv: x,y,z,roll,pitch,yaw,timestamp  | output.csv: aX,aY,aZ,gX,gY,gZ,timestamp |

## Units
The units used are compliant with the larger project this virtual IMU is part of.
| Value               | Units used in Project         |
|---------------------|-------------------------------|
| roll, pitch, yaw    | degrees (o)                   |
| pX, pY, pZ          | meters (m)                    |
| accX, accY, accZ    | g (1g = 9.81 m/s^2)           |
| gyroX, gyroY, gyroZ | degrees per seconds (dps) |

## The coordinate system

## TO-DO
 - [ ] Add the conversion from tracking points (position[pX,pY,pZ] + orientation[roll, pitch, yaw]) to sensor values(accelerometer[accX, accY, accZ] + gyroscope[gyroX, gyroY, gyroZ]).
    - [ ] Calculating the angular velocity from derivation of roll, pitch, yaw values. 
    - [ ] Calculation of acceleration in the x,y,z directions using double derivation of position values.
    - [ ] Conversion from linear acceleration calculation to accX, accY, accZ wit the rotation matrix.
    - [ ] Conversion of the gravity vector to propper accX, accY, accZ with the rotation matrix.
    - [ ] Addition of gravity to the obtained accX, accY, accZ values.
 - [ ] Add realistic noise to the obtained sensor values.