#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "lib/virtual_sensor/virtual_sensor.h"
#include "lib/util/util.h"

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
    data.roll = std::stof(tokens[3]);
    data.pitch = std::stof(tokens[4]);
    data.yaw = std::stof(tokens[5]);
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

    // Open the output file
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

    float rotationMatrix[3][3];
    bool lineFlag = true;

    // Read and process the lines from the input file
    std::getline(inputFile, line); // First line is the header.
    while (std::getline(inputFile, line))
    {

        // Step 1: Parse the line and extract the data.
        parseInputCSVLine(line, point);
        // Step 2: Insert the new tracking point into the virtual sensor.
        IMU.sampleObjectData(point);
        // Step 3: If new sensor data is available, update the output.
        if(IMU.isSensorDataAvailable()){

            IMU.getSensorData(outputSensorData);

            // Use the output as you wish. (Currently written to the output file.)
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
        


    }


    // Close the input and output files
    inputFile.close();
    outputFile.close();

    std::cout << "Data processing complete. Output saved to " << outputFilePath << std::endl;

    return 0;
}