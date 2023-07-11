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

