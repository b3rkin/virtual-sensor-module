#include <iostream>
#include "virtual_sensor.h"
#include "../util/util.h"



bool VirtualIMU::calculateLinearAcceleration(float acceleration[3]){

    // This calculation can be done only if there are at least three tracking points.
    if(trackingQ.size() < 3){return false;}

    // Get the last three tracking points.
    TrackingPoint point_t0, point_t1, point_t2;
    point_t0 = trackingQ[0];
    point_t1 = trackingQ[1];
    point_t2 = trackingQ[2];

    // Calculate the time deltasb and turn the into seconds.
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

void VirtualIMU::calculateVelocityFromPosition(TrackingPoint& newPoint, TrackingPoint& oldPoint){


}

// Convert Euler angles to rotation matrix
void VirtualIMU::updateRotationMatrix(float roll, float pitch, float yaw) {
    float cosRoll  = cos(roll);
    float sinRoll  = sin(roll);
    float cosPitch = cos(pitch);
    float sinPitch = sin(pitch);
    float cosYaw   = cos(yaw);
    float sinYaw   = sin(yaw);

    rotationMatrix[0][0] = cosPitch * cosYaw;
    rotationMatrix[0][1] = -cosRoll * sinYaw + sinRoll * sinPitch * cosYaw;
    rotationMatrix[0][2] = sinRoll * sinYaw + cosRoll * sinPitch * cosYaw;
    rotationMatrix[1][0] = cosPitch * sinYaw;
    rotationMatrix[1][1] = cosRoll * cosYaw + sinRoll * sinPitch * sinYaw;
    rotationMatrix[1][2] = -sinRoll * cosYaw + cosRoll * sinPitch * sinYaw;
    rotationMatrix[2][0] = -sinPitch;
    rotationMatrix[2][1] = sinRoll * cosPitch;
    rotationMatrix[2][2] = cosRoll * cosPitch;
}

void VirtualIMU::addGravityVectorToAcc(float& accX, float& accY, float& accZ){
        float gravity[3] = {0.0, 0.0, 1}; // (0g, 0g, 1g) This is the output of the a

    // Rotate gravity vector to the body frame
    float bodyGravity[3];
    for (int i = 0; i < 3; i++) {
        bodyGravity[i] = 0.0;
        for (int j = 0; j < 3; j++) {
            bodyGravity[i] += rotationMatrix[i][j] * gravity[j];
        }
    }

    // Add the gravity vector to the accelerometer output.
    accX += bodyGravity[0];
    accY += bodyGravity[1];
    accZ += bodyGravity[2];
}

void VirtualIMU::sampleObjectData(TrackingPoint newPoint){
    trackingQ.push_back(newPoint);
}

bool VirtualIMU::getSensorData(SensorOutput& output){
    if(sensorQ.empty()){ return false;}
    output = sensorQ.front();
    sensorQ.erase(sensorQ.begin());
    return true;
}

bool VirtualIMU::isSensorDataAvailable(){
    return !sensorQ.empty();
}

bool VirtualIMU::calculateSensorData(){

    // This calculation can be done only if there are at least three tracking points.
    if(trackingQ.size() < 3){return false;}

    // Step 1: Calculate the linear acceleration.
    float acceleration[3];
    if(!calculateLinearAcceleration(acceleration)){return false;};

    // Remove the oldest tracking point for new calculations.
    trackingQ.erase(trackingQ.begin());

    // Insert calculated sensor data into the sensor data FIFO.
    SensorOutput sensorData;
    sensorData.accX = acceleration[0];
    sensorData.accY = acceleration[1];
    sensorData.accZ = acceleration[2];
    sensorQ.push_back(sensorData);

    return true;
}






