#include "virtual_sensor.h"
#include "../util/util.h"


bool VirtualIMU::calculateLinearAcceleration(){

    // This calculation can be done only if there are at least three tracking points.
    if(trackingQ.size() < 3){return false;}

    // Get the last three tracking points.
    TrackingPoint point_t0, point_t1, point_t2;
    point_t0 = trackingQ[0];
    point_t1 = trackingQ[1];
    point_t2 = trackingQ[2];
    trackingQ.erase(trackingQ.begin()); // remove the first tracking point.

    // Calculate the time deltas.
    unsigned long deltaT1 = point_t1.timestamp - point_t0.timestamp;
    unsigned long deltaT2 = point_t2.timestamp - point_t1.timestamp;
    
    // Calculate the linear velocities v1 and v2.
    float velocity1[3] = {0,0,0}; // vX, vY, vZ
    velocity1[0] = (point_t1.pX - point_t0.pX) / (float) deltaT1;
    velocity1[1] = (point_t1.pY - point_t0.pY) / (float) deltaT1;
    velocity1[2] = (point_t1.pZ - point_t0.pZ) / (float) deltaT1;
    float velocity2[3] = {0,0,0}; // vX, vY, vZ
    velocity2[0] = (point_t2.pX - point_t1.pX) / (float) deltaT2;
    velocity2[1] = (point_t2.pY - point_t1.pY) / (float) deltaT2;
    velocity2[2] = (point_t2.pZ - point_t1.pZ) / (float) deltaT2;

    // Calculate the linear acceleration a1.
    float acceleration[3] = {0,0,0}; // aX, aY, aZ
    acceleration[0] = (velocity2[0] - velocity1[0]) / (float) (deltaT1/2+deltaT2/2);
    acceleration[1] = (velocity2[1] - velocity1[1]) / (float) (deltaT1/2+deltaT2/2);
    acceleration[2] = (velocity2[2] - velocity1[2]) / (float) (deltaT1/2+deltaT2/2);    

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
    if(!sensorQ.empty()){
        output = sensorQ.front();
        sensorQ.erase(sensorQ.begin());;
        return true;
    }
    return false;
}

bool VirtualIMU::isSensorDataAvailable(){

    return !sensorQ.empty();

}