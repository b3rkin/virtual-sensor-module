
#ifndef _UTIL_H_
#define _UTIL_H_

#define LOLI 345

void matrixMult3x3(float A[][3], float B[][3], float result[][3]);
void matrixVectorMult3x3(float matrix[][3], float vector[], float result[]);
void transposeMatrix(float matrix[][3], float result[][3]);

#endif /* _UTIL_H_ */