#include <vector>
#include <cmath>

#define GRAVITY (9.81) // m/s^2
#define PI (3.14159265359)
#define RAD2DEG (180.0/PI)
#define DEG2RAD (PI/180.0)
#define MAX_ABS_ANG_VEL (250) // dps | The maximum angular velocity of the object in degrees per second. 

struct TrackingPoint
{
    float pX, pY, pZ;
    float roll, pitch, yaw;
    unsigned long timestamp;
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
