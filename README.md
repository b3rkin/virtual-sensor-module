# The Project:
This project is a c++ program that takes position (x,y,z) and orientation (roll, pitch, yaw) data and turns it into realistic accelerometer and gyroscope data.

## Current status of the project:
The program iterates through the datapoints from the input csv and calculates the accelerometer and gyroscope values without any noise. After each calculation it inserts the sensor output to the output csv (With just the linear acceleration values). The program assumes that the inertial measurement unit (IMU) is placed exactly in the center of the object it is attached to.

## Setup
1. Run the included "compile_virtual_sensor_program.sh" script in the root of the directory to compile the virtual sensor module:
   - ./ compile_virtual_sensor_program.sh 
2. Run the program from the directory root:
   -  ./virtual_sensor_program

## I/O

| Input       | Output |
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
| gyroX, gyroY, gyroZ | degrees per seconds (dps)     |
| timestamp           | micro seconds (μs)            |

## Used Coordinat Systems

## TO-DO
 - [ ] Add the conversion from tracking points (position[pX,pY,pZ] + orientation[roll, pitch, yaw]) to sensor values(accelerometer[accX, accY, accZ] + gyroscope[gyroX, gyroY, gyroZ]).
    - [ x] Calculating the angular velocity from integration of roll, pitch, yaw values. 
    - [x] Calculation of acceleration in the x,y,z directions using double derivation of position values.
    - [ x] Conversion from linear acceleration calculation to accX, accY, accZ with the rotation matrix.
    - [x ] Conversion of the gravity vector to proper accX, accY, accZ with the rotation matrix.
    - [ x] Addition of gravity to the obtained accX, accY, accZ values.
 - [ ] Add realistic noise to the obtained sensor values.
 - [ ] Add IMU offset calibration settings.
 - [ ] Add the used coordinate system to README.
 - [ ] Bugfixes and fix angles.
 - [ ] Test the system outputs in its current setting.
 ## References
 [1]  Mitchell Tillman (2023). 3D Rigid Body Angular Velocity (https://github.com/mtillman14/Ref-Frames-Angular-Velocity/releases/tag/v1.1), GitHub. Retrieved June 14, 2023. 