#ifndef _3DFUNC_H_
#define _3DFUNC_H_

typedef struct{
    float x;
    float y;
    float z;
} Vector3;

Vector3 createVector3(float x, float y, float z);
void matmul(float matrix[3][3], Vector3 *vector);
float map(float value, float fromLow, float fromHigh, float toLow, float toHigh);
void projectPoint(Vector3* point3D, float distance);
Vector3 createSphere(int size, Vector3 globe[size][size], float r);
//create create rectangle, square and triangle (3d)
#endif