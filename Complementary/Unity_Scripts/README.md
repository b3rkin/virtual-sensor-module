# This Folder:
This folder contains scripts for the unity game engine in c# that can be used together with the virtual sensor module in the UDP mode. 

## KeyboardController.cs 
With this script it is possible to control the position of a rigidbody game object with the arrow keys on the X and Y axis and the Z and X keys to move on the +-Z axis.
### Setup
Attach the script to a rigid body object and adjust the speed variable as desired. 

## UDPSender.cs 
With this script it is possible to send the position and orientation of a gameobject in a UDP packet to an internal socket o
### Setup
Attach the script to the game object that you want to track.