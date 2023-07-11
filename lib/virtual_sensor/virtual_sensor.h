#ifndef _VIRTUAL_SENSOR_H_
#define _VIRTUAL_SENSOR_H_


#include <vector>
#include <cmath>
#include "../util/util.h"


#define GRAVITY (9.81) // m/s^2
#define PI (3.14159265359)
#define RAD2DEG (180.0/PI)
#define DEG2RAD (PI/180.0)
#define MAX_ABS_ANG_VEL (250) // dps | The maximum angular velocity of the object in degrees per second. 
#define ROT_ORDER "YXZ"  // This is the rotation convention the sensor uses for euler rotation.


struct TrackingPoint
{
    float pX, pY, pZ;
    float rotX, rotY, rotZ;
    float angvX, angvY, angvZ;
    unsigned int timestamp;

};



struct SensorOutput
{
    float accX, accY, accZ;
    float gyroX, gyroY, gyroZ;
    unsigned long timestamp;
};

class VirtualIMU
{
    public:
    
        VirtualIMU();
        void sampleObjectData(TrackingPoint newPoint);
        bool isSensorDataAvailable();
        bool getSensorData(SensorOutput &output);
        bool calculateSensorData();

    private:

        std::vector<TrackingPoint> trackingQ;
        std::vector<SensorOutput> sensorQ;

        float rotationMatrix[3][3];

        bool calculateAngularVelocity(float angVel[3]);
        bool updateRotationMatrix();
        bool calculateLinearAcceleration(float acceleration[3]);
        void calculateAccelerometerValues(float acceleration[3], float accelerometer[3]);
};


#endif // _VIRTUAL_SENSOR_H_