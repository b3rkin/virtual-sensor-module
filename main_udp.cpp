#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "lib/virtual_sensor/virtual_sensor.h"


#define RECEIVE_PORT 1234
#define SEND_PORT 5678
#define BUFFER_SIZE 1024
#define DESTINATION_IP "127.0.0.1" // IP Address of the destination.




int main() {

    // Create a UDP socket
    int sock = socket(AF_INET, SOCK_DGRAM, 0);

    if (sock < 0) {
        std::cerr << "Failed to create socket." << std::endl;
        return 1;
    }

    // Bind the socket to the specified receiving port.
    sockaddr_in receiveAddr;
    receiveAddr.sin_family = AF_INET;
    receiveAddr.sin_port = htons(RECEIVE_PORT);
    receiveAddr.sin_addr.s_addr = INADDR_ANY;
    socklen_t addrLen = sizeof(receiveAddr);
    if (bind(sock, (struct sockaddr*)&receiveAddr, sizeof(receiveAddr)) < 0) {
        std::cerr << "Failed to bind socket." << std::endl;
        return 1;
    }

    // Set up the destination address.
    sockaddr_in destination = { 0 };
    destination.sin_family = AF_INET;
    destination.sin_port = htons(SEND_PORT);
    inet_pton(AF_INET, DESTINATION_IP, &destination.sin_addr);
    

    // Buffer for receiving and sending data.
    char receiveBuffer[BUFFER_SIZE];
    char sendBuffer[BUFFER_SIZE];

    // Initialize the first time stamp.
    unsigned long lastTimeStamp = 0;


    TrackingPoint point;
    SensorOutput outputSensorData;
    VirtualIMU IMU;


    while (true) {

        // Receive UDP packet
        ssize_t numBytes = recvfrom(sock, receiveBuffer, BUFFER_SIZE, 0, (struct sockaddr*)&receiveAddr, &addrLen);

        if (numBytes < 0) {
            std::cerr << "Failed to receive packet." << std::endl;
            continue;
        }

        // Parse the received message
        std::memcpy(&point, receiveBuffer, sizeof(TrackingPoint));

        // std::cout<<sizeof(TrackingPoint)<<std::endl;


        // Calculate sample period. ()
        float samplePeriod = (float)(point.timestamp - lastTimeStamp)/(float)1000000;
        lastTimeStamp = point.timestamp;

        // Perform calculations with the data
        std::cout << "Received UDP packet:" << std::endl;
        std::cout << "rotX: " << point.rotX << std::endl;
        std::cout << "rotY: " << point.rotY << std::endl;
        std::cout << "rotZ: " << point.rotZ << std::endl;
        std::cout << "pX: " << point.pX << std::endl;
        std::cout << "pY: " << point.pY << std::endl;
        std::cout << "pZ: " << point.pZ << std::endl;
        std::cout << "angVX: " << point.angvX << std::endl;
        std::cout << "angVY: " << point.angvY << std::endl;
        std::cout << "angVZ: " << point.angvZ << std::endl;
        std::cout << "velX: " << point.velX << std::endl;
        std::cout << "velY: " << point.velY << std::endl;
        std::cout << "velZ: " << point.velZ << std::endl;
        std::cout << "Timestamp: " << point.timestamp << std::endl;
        std::cout << "deltaTime: " << samplePeriod << std::endl;
        std::cout << std::endl;

        // Sensor processing Starts here:
        // Step 1: Insert new tracking point into the virtual sensor.
        IMU.sampleObjectData(point);

        // Step 2: If new sensor data is not available try calculating a new one and if it cannot then wait for a new tracking point.
        if(!IMU.isSensorDataAvailable()){ 

            if(!IMU.calculateSensorData()){ // If the sensor data cannot be calculated then continue to sample a new tracking point.
                continue;
            }
            else{IMU.getSensorData(outputSensorData);}
        }
        else{
            IMU.getSensorData(outputSensorData);
        }

        // Step 3: Send the sensor data to the destination.
        std::memcpy(sendBuffer, &outputSensorData, sizeof(outputSensorData));

        ssize_t sendBytes = sendto(sock, sendBuffer, sizeof(sendBuffer), 0, (struct sockaddr*)&destination, sizeof(destination));
        if (sendBytes < 0) {
            std::cerr << "Failed to send packet." << std::endl;
        }
    }


    return 0;
}