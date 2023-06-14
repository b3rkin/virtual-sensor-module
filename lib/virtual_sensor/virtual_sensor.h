#include <vector>
#include <cmath>

#define GRAVITY (9.81) // m/s^2

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
    void sampleObjectData(TrackingPoint newPoint);
    bool calculateLinearAcceleration(float acceleration[3]);
    void calculateVelocityFromPosition(TrackingPoint &newPoint, TrackingPoint &oldPoint);
    bool isSensorDataAvailable();
    void addGravityVectorToAcc(float &accX, float &accY, float &accZ);
    bool getSensorData(SensorOutput &output);
    bool calculateSensorData();

private:
    std::vector<TrackingPoint> trackingQ;
    std::vector<SensorOutput> sensorQ;
    float rotationMatrix[3][3];

    void updateRotationMatrix(float roll, float pitch, float yaw);
};
