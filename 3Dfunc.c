#include "3Dfunc.h"
#include <math.h>

Vector3 createVector3(float x, float y, float z){
    Vector3 vec;
    vec.x = x;
    vec.y = y;
    vec.z = z;
    return vec;
}

void matmul(float matrix[3][3], Vector3 *vector) {
   float x = vector->x;
   float y = vector->y;
   float z = vector->z;

   vector->x = matrix[0][0] * x + matrix[0][1] * y + matrix[0][2] * z;
   vector->y = matrix[1][0] * x + matrix[1][1] * y + matrix[1][2] * z;
   vector->z = matrix[2][0] * x + matrix[2][1] * y + matrix[2][2] * z;
}

float map(float value, float fromLow, float fromHigh, float toLow, float toHigh) {
    // Ensure that the value is within the input range
    value = (value < fromLow) ? fromLow : value;
    value = (value > fromHigh) ? fromHigh : value;

    // Calculate the mapped value
    float mappedValue = ((value - fromLow) / (fromHigh - fromLow)) * (toHigh - toLow) + toLow;

    return mappedValue;
}