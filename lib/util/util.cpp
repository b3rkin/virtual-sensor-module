#include "util.h"



// Function to multiply two matrices
void matrixMult3x3(float A[][3], float B[][3], float result[][3]) {
    // Perform matrix multiplication
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            result[i][j] = 0;
            for (int k = 0; k < 3; k++) {
                result[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

// Function to multiply two matrices
// Multiplies a 3x3 matrix with a 3x1 vector as in Mat x Vec.
void matrixVectorMult3x3(float matrix[][3], float vector[], float result[]) {
    // Perform matrix multiplication
        for (int i = 0; i < 3; i++) {
            result[i] = 0;
            for (int j = 0; j < 3; j++) {
            result[i] += matrix[i][j] * vector[j];
        }
    }
}

// Function to calculate the transpose of a 3x3 matrix
void transposeMatrix(float matrix[][3], float result[][3]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            result[j][i] = matrix[i][j];
        }
    }
}


float addWhiteNoise(float noise_intensity) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<float> distribution(0.0, noise_intensity);
    float noise = distribution(gen);
    return noise;
}

