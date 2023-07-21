#include <iostream>
#include "virtual_sensor.h"




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
    float acceleration[3] = {0,0,0};
    if(!calculateLinearAcceleration(acceleration)){return false;};

    // Step 4: Adjust the linear acceleration to actual accelerometer values.
    float accelerometer[3] = {0,0,0};
    calculateAccelerometerValues(acceleration, accelerometer);

    // Step 5: Insert calculated sensor data into the sensor data FIFO.
    SensorOutput sensorData;
    // sensorData.accX = accelerometer[0];
    // sensorData.accY = accelerometer[1];
    // sensorData.accZ = accelerometer[2];
    // sensorData.gyroX = angVel[0];
    // sensorData.gyroY = angVel[1];
    // sensorData.gyroZ = angVel[2];
    sensorData.accX = accelerometer[0] + addWhiteNoise(WHITE_NOISE_INT_ACC);
    sensorData.accY = accelerometer[1] + addWhiteNoise(WHITE_NOISE_INT_ACC);
    sensorData.accZ = accelerometer[2] + addWhiteNoise(WHITE_NOISE_INT_ACC);
    sensorData.gyroX = angVel[0]+ addWhiteNoise(WHITE_NOISE_INT_GYRO);
    sensorData.gyroY = angVel[1]+ addWhiteNoise(WHITE_NOISE_INT_GYRO);
    sensorData.gyroZ = angVel[2]+ addWhiteNoise(WHITE_NOISE_INT_GYRO);
    if (trackingQ[0].timestamp - depthCameraCounter > (1/DEPTH_CAMERA_FREQ)*1000000){
        depthCameraCounter = trackingQ[0].timestamp;
        sensorData.depthFlag = true;
        sensorData.pX    = trackingQ[0].pX;
        sensorData.pY    = trackingQ[0].pY;
        sensorData.pZ    = trackingQ[0].pZ;
        sensorData.rotX  = trackingQ[0].rotX;
        sensorData.rotY  = trackingQ[0].rotY;
        sensorData.rotZ  = trackingQ[0].rotZ;
    }
    else{
        sensorData.depthFlag = false;
        sensorData.pX    = 0;
        sensorData.pY    = 0;
        sensorData.pZ    = 0;
        sensorData.rotX  = 0;
        sensorData.rotY  = 0;
        sensorData.rotZ  = 0;
    }

    // sensorData.timestamp = (trackingQ[0].timestamp + trackingQ[1].timestamp)/2;
    sensorData.timestamp = trackingQ[1].timestamp;
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
    float newDeltaT = (deltaT1 + deltaT2) / 2;

    // Calculate the linear velocities v1 and v2.
    // float velocity1[3] = {0,0,0}; // vX, vY, vZ
    // velocity1[0] = (point_t1.pX - point_t0.pX) / deltaT1;
    // velocity1[1] = (point_t1.pY - point_t0.pY) / deltaT1;
    // velocity1[2] = (point_t1.pZ - point_t0.pZ) / deltaT1;
    // float velocity2[3] = {0,0,0}; // vX, vY, vZ
    // velocity2[0] = (point_t2.pX - point_t1.pX) / deltaT2;
    // velocity2[1] = (point_t2.pY - point_t1.pY) / deltaT2;
    // velocity2[2] = (point_t2.pZ - point_t1.pZ) / deltaT2;

    // Calculate the linear acceleration.
    // acceleration[0] = (velocity2[0] - velocity1[0]) / newDeltaT;
    // acceleration[1] = (velocity2[1] - velocity1[1]) / newDeltaT;
    // acceleration[2] = (velocity2[2] - velocity1[2]) / newDeltaT;    

    acceleration[0] = (point_t1.velX - point_t0.velX ) / deltaT1;
    acceleration[1] = (point_t1.velY - point_t0.velY ) / deltaT1;
    acceleration[2] = (point_t1.velZ - point_t0.velZ ) / deltaT1;   

    acceleration[0] /= GRAVITY;
    acceleration[1] /= GRAVITY;
    acceleration[2] /= GRAVITY;

    return true;
}


bool VirtualIMU::updateRotationMatrix() {

    if(trackingQ.size() < 2){return false;}

    float Rx[3][3], Ry[3][3], Rz[3][3];  
    float RxT[3][3], RyT[3][3], RzT[3][3];  

    float roll  = trackingQ[1].rotX*DEG2RAD;
    float pitch = trackingQ[1].rotY*DEG2RAD;
    float yaw   = trackingQ[1].rotZ*DEG2RAD;
    
    float cosRoll  = cos(roll);
    float sinRoll  = sin(roll);
    float cosPitch = cos(pitch);
    float sinPitch = sin(pitch);
    float cosYaw   = cos(yaw);
    float sinYaw   = sin(yaw);

    // Transposes of the rotation matrices.
    RxT[0][0] = 1;
    RxT[0][1] = 0;
    RxT[0][2] = 0;
    RxT[1][0] = 0;
    RxT[1][1] = cosRoll;
    RxT[1][2] = sinRoll;
    RxT[2][0] = 0;
    RxT[2][1] = -sinRoll;
    RxT[2][2] = cosRoll;

    RyT[0][0] = cosPitch;
    RyT[0][1] = 0;
    RyT[0][2] = -sinPitch;
    RyT[1][0] = 0;
    RyT[1][1] = 1;
    RyT[1][2] = 0;
    RyT[2][0] = sinPitch;
    RyT[2][1] = 0;
    RyT[2][2] = cosPitch;

    RzT[0][0] = cosYaw;
    RzT[0][1] = sinYaw;
    RzT[0][2] = 0;
    RzT[1][0] = -sinYaw;
    RzT[1][1] = cosYaw;
    RzT[1][2] = 0;   
    RzT[2][0] = 0;
    RzT[2][1] = 0;
    RzT[2][2] = 1;


    // The rotation matrices
    // Rx[0][0] = 1;
    // Rx[0][1] = 0;
    // Rx[0][2] = 0;
    // Rx[1][0] = 0;
    // Rx[1][1] = cosRoll;
    // Rx[1][2] = -sinRoll;
    // Rx[2][0] = 0;
    // Rx[2][1] = sinRoll;
    // Rx[2][2] = cosRoll;

    // Ry[0][0] = cosPitch;
    // Ry[0][1] = 0;
    // Ry[0][2] = sinPitch;
    // Ry[1][0] = 0;
    // Ry[1][1] = 1;
    // Ry[1][2] = 0;
    // Ry[2][0] = -sinPitch;
    // Ry[2][1] = 0;
    // Ry[2][2] = cosPitch;

    // Rz[0][0] = cosYaw;
    // Rz[0][1] = -sinYaw;
    // Rz[0][2] = 0;
    // Rz[1][0] = sinYaw;
    // Rz[1][1] = cosYaw;
    // Rz[1][2] = 0;   
    // Rz[2][0] = 0;
    // Rz[2][1] = 0;
    // Rz[2][2] = 1;


    if (ROT_ORDER == "YXZ"){
        float tempMat[3][3];
        matrixMult3x3(RyT, RxT, tempMat);
        matrixMult3x3(tempMat, RzT, rotationMatrix);

    }
    else if(ROT_ORDER == "XYZ"){
        float tempMat[3][3];
        matrixMult3x3(RxT, RyT, tempMat);
        matrixMult3x3(tempMat, RzT, rotationMatrix);
    }
    else{// The number of orders can be added here if needed.
    
    }

    return true;
}


bool VirtualIMU::calculateAngularVelocity(float angVel[3]){

    // This calculation can be done only if there are at least two tracking points.
    if(trackingQ.size() < 2){return false;}

    // Get the last two tracking points.
    TrackingPoint point_t0, point_t1;
    // point_t0 = trackingQ[0];
    point_t1 = trackingQ[1];

    // // Calculate the time delta and turn the into seconds.
    // float deltaT = (float) (point_t1.timestamp - point_t0.timestamp)/1000000;

    // // Calculate the difference in angles
    // float euler_delta[3];
    // euler_delta[0] = (point_t1.rotX - point_t0.rotX)/ deltaT;
    // euler_delta[1] = (point_t1.rotY - point_t0.rotY)/ deltaT;
    // euler_delta[2] = (point_t1.rotZ - point_t0.rotZ)/ deltaT;

    // euler_delta[0] = (-point_t1.rotX + point_t0.rotX)/ deltaT;
    // euler_delta[1] = (-point_t1.rotY + point_t0.rotY)/ deltaT;
    // euler_delta[2] = (-point_t1.rotZ + point_t0.rotZ)/ deltaT;


    // Calculate necessary components. (roll, pitch, yaw; rotX, rotY, rotZ; theta, phi, psi)
    // float sinTheta = sin((360-point_t1.rotX)*DEG2RAD);
    // float sinPsi = sin((360-point_t1.rotZ)*DEG2RAD);
    // float cosTheta = cos((360-point_t1.rotX)*DEG2RAD);
    // float cosPsi = cos((360 - point_t1.rotZ)*DEG2RAD);

    // Check if a 360 mark has been surpassed. 
    // if(angVel_tmp[0] > MAX_ABS_ANG_VEL){angVel_tmp[0] -= 360;}
    // if(angVel_tmp[0] < -MAX_ABS_ANG_VEL){angVel_tmp[0] += 360;}
    // if(angVel_tmp[1] > MAX_ABS_ANG_VEL){angVel_tmp[1] -= 360;}
    // if(angVel_tmp[1] < -MAX_ABS_ANG_VEL){angVel_tmp[1] += 360;}
    // if(angVel_tmp[2] > MAX_ABS_ANG_VEL){angVel_tmp[2] -= 360;}
    // if(angVel_tmp[2] < -MAX_ABS_ANG_VEL){angVel_tmp[2] += 360;}

    // Calculate the angular velocity in deg/sec.
    // angVel[0] = euler_delta[1]*sinTheta*sinPsi + euler_delta[0]*cosPsi ;
    // angVel[1] = euler_delta[1]*sinTheta*cosPsi - euler_delta[0]*sinPsi;
    // angVel[2] = euler_delta[1]*cosTheta + euler_delta[2];

    angVel[0] = point_t1.angvX*RAD2DEG;
    angVel[1] = point_t1.angvY*RAD2DEG;
    angVel[2] = point_t1.angvZ*RAD2DEG;

    return true;
}


void VirtualIMU::calculateAccelerometerValues(float acceleration[3], float accelerometer[3]){

    // Obtain acceleration in the body frame.
    matrixVectorMult3x3(rotationMatrix, acceleration, accelerometer);
    std::cout<<std::endl;

    float gravity[3] = {0.0, 0.0, 1}; // (0g, 0g, 1g) This is the output of the a
    float bodyGravity[3] = {0.0, 0.0, 0.0};

    // Obtain gravity vector to the body frame.
    matrixVectorMult3x3(rotationMatrix, gravity, bodyGravity);

    // Add the gravity vector to the accelerometer output.
    accelerometer[0] += bodyGravity[0];
    accelerometer[1] += bodyGravity[1];
    accelerometer[2] += bodyGravity[2];

}




