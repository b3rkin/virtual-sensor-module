#include <vector>
#include <cmath>
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




class VirtualIMU{
    
    public:

        std::vector<TrackingPoint> trackingQ;
        std::vector<SensorOutput> sensorQ;
        float rotationMatrix[3][3];

        void sampleObjectData(TrackingPoint newPoint);
        bool calculateLinearAcceleration();
        void calculateVelocityFromPosition(TrackingPoint& newPoint, TrackingPoint& oldPoint);
        bool isSensorDataAvailable();
        void addGravityVectorToAcc(float& accX, float& accY, float& accZ);
        bool getSensorData(SensorOutput& output);
        void updateRotationMatrix(float roll, float pitch, float yaw);

    private:

 
};
