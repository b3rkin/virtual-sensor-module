# This Folder:
This folder contains scripts for the unity game engine in c# that can be used together with the virtual sensor module in the UDP mode. 

## KeyboardController.cs 
With this script it is possible to control the position of a rigidbody game object with the arrow keys on the X and Y axis and the Z and X keys to move on the +-Z axis.
### Setup
Attach the script to a rigid body object and adjust the speed variable as desired. 

## UDPSender.cs 
With this script it is possible to send the position and orientation of a gameobject in a UDP packet to an internal socket which the udp program is monitoring. The axis of unity are sent in a diffrent order to match the axis with the sensor module.
### Setup
Attach the script to the game object that you want to track.

## ObjectRotator.cs 
This script can continuously rotate the game object on any axis with any speed in deg/sec. It is used especially for debugging the gyroscope of the virtual sensor module. Disabling gravity of the rigidbody is advised. The axis are set up in accordance with the sensor not unity.
### Setup
Attach the script to the game object that you want to rotate. Select the required axes and speeds.