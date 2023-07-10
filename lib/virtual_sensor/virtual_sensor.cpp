#include <iostream>
#include "virtual_sensor.h"
#include "../util/util.h"



VirtualIMU::VirtualIMU(){
    // Initialize the rotation matrix as an identity matrix.
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            if(i==j){rotationMatrix[i][j] = 1;}
            else{rotationMatrix[i][j] = 0;}
        }
    }
}


void VirtualIMU::sampleObjectData(TrackingPoint newPoint){
    trackingQ.push_back(newPoint);
}


bool VirtualIMU::isSensorDataAvailable(){
    return !sensorQ.empty();
}


bool VirtualIMU::getSensorData(SensorOutput& output){
    if(sensorQ.empty()){return false;}
    output = sensorQ[0];
    sensorQ.erase(sensorQ.begin());
    return true;
}


bool VirtualIMU::calculateSensorData(){

    // This calculation can be done only if there are at least three tracking points.
    if(trackingQ.size() < 3){return false;}

    // Step 1: Calculate angular velocity.
    float angVel[3] = {0,0,0};
    if(!calculateAngularVelocity(angVel)){return false;}

    // Step 2: Update the rotation matrix to be able to get rotation. (With the orientation at tracking point 2.)
    if(!updateRotationMatrix()){return false;}

    // Step 3: Calculate linear acceleration.
    float acceleration[3];
    if(!calculateLinearAcceleration(acceleration)){return false;};

    // Step 4: Adjust the linear acceleration to actual accelerometer values.
    float accelerometer[3] = {0,0,0};
    calculateAccelerometerValues(acceleration, accelerometer);

    // Step 5: Insert calculated sensor data into the sensor data FIFO.
    SensorOutput sensorData;
    sensorData.accX = accelerometer[0];
    sensorData.accY = accelerometer[1];
    sensorData.accZ = accelerometer[2];
    sensorData.gyroX = angVel[0];
    sensorData.gyroY = angVel[1];
    sensorData.gyroZ = angVel[2];
    sensorData.timestamp = (trackingQ[0].timestamp + trackingQ[1].timestamp)/2;
    sensorQ.push_back(sensorData);

    // Step 6: Remove the oldest tracking point for new calculations.
    trackingQ.erase(trackingQ.begin());

    return true;
}


bool VirtualIMU::calculateLinearAcceleration(float acceleration[3]){

    // This calculation can be done only if there are at least three tracking points.
    if(trackingQ.size() < 3){return false;}

    // Get the last three tracking points.
    TrackingPoint point_t0, point_t1, point_t2;
    point_t0 = trackingQ[0];
    point_t1 = trackingQ[1];
    point_t2 = trackingQ[2];

    // Calculate the time deltas and turn the into seconds.
    float deltaT1 = (float) (point_t1.timestamp - point_t0.timestamp)/1000000;
    float deltaT2 = (float) (point_t2.timestamp - point_t1.timestamp)/1000000;

    // Calculate the linear velocities v1 and v2.
    float velocity1[3] = {0,0,0}; // vX, vY, vZ
    velocity1[0] = (point_t1.pX - point_t0.pX) / deltaT1;
    velocity1[1] = (point_t1.pY - point_t0.pY) / deltaT1;
    velocity1[2] = (point_t1.pZ - point_t0.pZ) / deltaT1;
    float velocity2[3] = {0,0,0}; // vX, vY, vZ
    velocity2[0] = (point_t2.pX - point_t1.pX) / deltaT2;
    velocity2[1] = (point_t2.pY - point_t1.pY) / deltaT2;
    velocity2[2] = (point_t2.pZ - point_t1.pZ) / deltaT2;

    // Calculate the linear acceleration.
    acceleration[0] = (velocity2[0] - velocity1[0]) / ((deltaT1 + deltaT2) / 2);
    acceleration[1] = (velocity2[1] - velocity1[1]) / ((deltaT1 + deltaT2) / 2);
    acceleration[2] = (velocity2[2] - velocity1[2]) / ((deltaT1 + deltaT2) / 2);    

    acceleration[0] /= GRAVITY;
    acceleration[1] /= GRAVITY;
    acceleration[2] /= GRAVITY;

    return true;
}


bool VirtualIMU::updateRotationMatrix() {

    if(trackingQ.size() < 2){return false;}

    float roll = trackingQ[1].roll;
    float pitch = trackingQ[1].pitch;
    float yaw = trackingQ[1].yaw;
    
    float cosRoll  = cos(roll);
    float sinRoll  = sin(roll);
    float cosPitch = cos(pitch);
    float sinPitch = sin(pitch);
    float cosYaw   = cos(yaw);
    float sinYaw   = sin(yaw);

    rotationMatrix[0][0] = cosPitch * cosYaw;
    rotationMatrix[0][1] = -cosRoll * sinYaw + sinRoll * sinPitch * cosYaw;
    rotationMatrix[0][2] = sinRoll  * sinYaw + cosRoll * sinPitch * cosYaw;
    rotationMatrix[1][0] = cosPitch * sinYaw;
    rotationMatrix[1][1] = cosRoll  * cosYaw + sinRoll * sinPitch * sinYaw;
    rotationMatrix[1][2] = -sinRoll * cosYaw + cosRoll * sinPitch * sinYaw;
    rotationMatrix[2][0] = -sinPitch;
    rotationMatrix[2][1] = sinRoll * cosPitch;
    rotationMatrix[2][2] = cosRoll * cosPitch;

    return true;
}


bool VirtualIMU::calculateAngularVelocity(float angVel[3]){

    // This calculation can be done only if there are at least two tracking points.
    if(trackingQ.size() < 2){return false;}

    // Get the last two tracking points.
    TrackingPoint point_t0, point_t1;
    point_t0 = trackingQ[0];
    point_t1 = trackingQ[1];

    // Calculate the time delta and turn the into seconds.
    float deltaT = (float) (point_t1.timestamp - point_t0.timestamp)/1000000;

    // Calculate the difference in angles
    float angVel_tmp[3];
    angVel_tmp[0] = (point_t1.roll  - point_t0.roll);
    angVel_tmp[1] = (point_t1.pitch - point_t0.pitch);
    angVel_tmp[2] = (point_t1.yaw   - point_t0.yaw);

    // Check if a 360 mark has been surpassed. 
    if(angVel_tmp[0] > MAX_ABS_ANG_VEL){angVel_tmp[0] -= 360;}
    if(angVel_tmp[0] < -MAX_ABS_ANG_VEL){angVel_tmp[0] += 360;}
    if(angVel_tmp[1] > MAX_ABS_ANG_VEL){angVel_tmp[1] -= 360;}
    if(angVel_tmp[1] < -MAX_ABS_ANG_VEL){angVel_tmp[1] += 360;}
    if(angVel_tmp[2] > MAX_ABS_ANG_VEL){angVel_tmp[2] -= 360;}
    if(angVel_tmp[2] < -MAX_ABS_ANG_VEL){angVel_tmp[2] += 360;}

    // Calculate the angular velocity in deg/sec.
    angVel[0] = angVel_tmp[0] / deltaT;
    angVel[1] = angVel_tmp[1] / deltaT;
    angVel[2] = angVel_tmp[2] / deltaT;

    return true;
}


void VirtualIMU::calculateAccelerometerValues(float acceleration[3], float accelerometer[3]){

    // Rotate the linear acceleration values to the body frame of the sensor to get actual accelerometer values.
    for(int i = 0; i < 3; i++){
        for(int j= 0; j < 3; j++){
            accelerometer[i] += rotationMatrix[i][j] * acceleration[j];
        }
    }

    float gravity[3] = {0.0, 0.0, 1}; // (0g, 0g, 1g) This is the output of the a
    float bodyGravity[3];

    // Add gravity vector to the accelerometer values.
    // Rotate gravity vector to the body frame
    for (int i = 0; i < 3; i++) {
        bodyGravity[i] = 0.0;
        for (int j = 0; j < 3; j++) {
            bodyGravity[i] += rotationMatrix[i][j] * gravity[j];
        }
    }

    // Add the gravity vector to the accelerometer output.
    accelerometer[0] += bodyGravity[0];
    accelerometer[1] += bodyGravity[1];
    accelerometer[2] += bodyGravity[2];


}




