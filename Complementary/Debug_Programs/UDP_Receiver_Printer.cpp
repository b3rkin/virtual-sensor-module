#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>

#define RECV_PORT 5678

struct SensorOutput {
    float accX, accY, accZ;
    float gyroX, gyroY, gyroZ;
    unsigned long timestamp;
};

int main() {
    int socketFD;
    struct sockaddr_in serverAddress, clientAddress;
    socklen_t clientLength = sizeof(clientAddress);
    char buffer[sizeof(SensorOutput)];

    // Create socket
    socketFD = socket(AF_INET, SOCK_DGRAM, 0);
    if (socketFD < 0) {
        std::cerr << "Error creating socket" << std::endl;
        return 1;
    }

    // Set server address
    memset((char*)&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(RECV_PORT);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    // Bind socket to port
    if (bind(socketFD, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        std::cerr << "Error binding socket" << std::endl;
        return 1;
    }

    // Receive and print packets continuously
    while (true) {
        ssize_t receivedBytes = recvfrom(socketFD, buffer, sizeof(buffer), 0, (struct sockaddr*)&clientAddress, &clientLength);
        if (receivedBytes == sizeof(SensorOutput)) {
            SensorOutput* data = reinterpret_cast<SensorOutput*>(buffer);
            std::cout << "Received SensorOutput struct:" << std::endl;
            std::cout << "accX: " << data->accX << std::endl;
            std::cout << "accY: " << data->accY << std::endl;
            std::cout << "accZ: " << data->accZ << std::endl;
            std::cout << "gyroX: " << data->gyroX << std::endl;
            std::cout << "gyroY: " << data->gyroY << std::endl;
            std::cout << "gyroZ: " << data->gyroZ << std::endl;
            std::cout << "timestamp: " << data->timestamp << std::endl << std::endl;
        } else {
            std::cerr << "Error receiving packet" << std::endl;
        }
    }
    
    return 0;
}