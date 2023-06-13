

struct TrackingPoints
{

    float pX, pY, pZ,
        roll, pitch, yaw;

    unsigned long timestamp;
};

struct SensorOutput
{

    float accX, accY, accZ;
    float gyroX, gyroY, gyroZ;

    unsigned long timestamp;
};