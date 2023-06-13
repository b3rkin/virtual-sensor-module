#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "lib/virtual_sensor/virtual_sensor.h"

void parseInputCSVLine(const std::string& line, TrackingPoints& data){
    std::istringstream ss(line);
    std::string token;
    std::vector<std::string> tokens;

    while (std::getline(ss, token, ',')) {
        tokens.push_back(token);
    }

    // Create an instance of the Data structure and populate its members

    data.pX        = std::stof(tokens[0]);
    data.pY        = std::stof(tokens[1]);
    data.pZ        = std::stof(tokens[2]);
    data.roll      = std::stof(tokens[3]);
    data.pitch     = std::stof(tokens[4]);
    data.yaw       = std::stof(tokens[5]);
    data.timestamp = std::stof(tokens[6]);
}


int main(){

    // Define the input and output file paths.
    std::string inputFilePath  = "io/input.csv";
    std::string outputFilePath = "io/output.csv";

    // Open the input file.
    std::ifstream inputFile(inputFilePath);

    if (!inputFile.is_open()) {
        std::cerr << "Failed to open input file." << std::endl;
        return 1;
    }

    // Open the output file
    std::ofstream outputFile(outputFilePath);
    if (!outputFile.is_open()) {
        std::cerr << "Failed to open output file." << std::endl;
        inputFile.close();  // Close the input file before exiting
        return 1;
    }

    std::string lineNew;
    std::string lineOld;
    TrackingPoints pointNew;
    TrackingPoints pointOld;
    bool lineFlag = true;

    // Read and process the lines from the input file
    std::getline(inputFile, lineNew); // First line is the header.
    while (std::getline(inputFile, lineNew)) {

        // Skip the first line. 
        if (lineFlag) {
            lineOld = lineNew;
            parseInputCSVLine(lineOld, pointOld);
            lineFlag = false;
            continue;
        }

        // Parse the line and extract the data
        parseInputCSVLine(lineNew, pointNew);

        // Process the data as needed (perform calculations, etc.)

        // Create a new line to be written to the output file
        std::string outputLine =  std::to_string(pointNew.pX) + "," 
                                + std::to_string(pointNew.pY) + "," 
                                + std::to_string(pointNew.pZ) + "," 
                                + std::to_string(pointNew.roll) + "," 
                                + std::to_string(pointNew.pitch) + ","
                                + std::to_string(pointNew.yaw) + ","  
                                + std::to_string(pointNew.timestamp) + "\n"; 

        // Write the line to the output file
        outputFile << outputLine;
    }

    // Close the input and output files
    inputFile.close();
    outputFile.close();

    std::cout << "Data processing complete. Output saved to " << outputFilePath << std::endl;

    return 0;
}