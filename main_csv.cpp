#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "lib/virtual_sensor/virtual_sensor.h"


void parseInputCSVLine(const std::string &line, TrackingPoint &data)
{
    std::istringstream ss(line);
    std::string token;
    std::vector<std::string> tokens;

    while (std::getline(ss, token, ','))
    {
        tokens.push_back(token);
    }

    // Create an instance of the Data structure and populate its members

    data.pX = std::stof(tokens[0]);
    data.pY = std::stof(tokens[1]);
    data.pZ = std::stof(tokens[2]);
    data.rotX = std::stof(tokens[3]);
    data.rotY = std::stof(tokens[4]);
    data.rotZ = std::stof(tokens[5]);
    data.timestamp = std::stof(tokens[6]);
}

int main()
{

    // Define the input and output file paths.
    std::string inputFilePath = "io/input.csv";
    std::string outputFilePath = "io/output.csv";

    // Open the input file.
    std::ifstream inputFile(inputFilePath);
    if (!inputFile.is_open())
    {
        std::cerr << "Failed to open input file." << std::endl;
        return 1;
    }

    // Open the output file.
    std::ofstream outputFile(outputFilePath);
    if (!outputFile.is_open())
    {
        std::cerr << "Failed to open output file." << std::endl;
        inputFile.close(); // Close the input file before exiting
        return 1;
    }

    std::string line;
    TrackingPoint point;
    SensorOutput outputSensorData;
    VirtualIMU IMU;

    // Read and process the lines from the input file
    std::getline(inputFile, line); // First line is the header.
    while (std::getline(inputFile, line)) // This line can/should be changed to any input method in further iterations.
    {
        // Step 1: Parse the line and extract the data.
        parseInputCSVLine(line, point);
        // Step 2: Insert the new tracking point into the virtual sensor.
        IMU.sampleObjectData(point);
        // Step 3: If new sensor data is not available try calculating a new one.
        if(!IMU.isSensorDataAvailable()){ 

            if(!IMU.calculateSensorData()){ // If the sensor data cannot be calculated then continue to sample a new tracking point.
                continue;
            }
            else{IMU.getSensorData(outputSensorData);}
        }
        else{
            IMU.getSensorData(outputSensorData);
        }

        // Step 4: Do something with the sensor data.
        // Currently written to the output file.
        // Create a new line to be written to the output file
        std::string outputLine = std::to_string(outputSensorData.accX)  + "," 
                               + std::to_string(outputSensorData.accY)  + "," 
                               + std::to_string(outputSensorData.accZ)  + "," 
                               + std::to_string(outputSensorData.gyroX) + "," 
                               + std::to_string(outputSensorData.gyroY) + "," 
                               + std::to_string(outputSensorData.gyroZ) + "," 
                               + std::to_string(outputSensorData.timestamp) + "\n";

        // Write the line to the output file
        outputFile << outputLine;
    }

    // Close the input and output files
    inputFile.close();
    outputFile.close();

    std::cout << "Data processing complete. Output saved to " << outputFilePath << std::endl;

    return 0;
}