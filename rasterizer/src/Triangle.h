#ifndef __TRIANGLE_H__
#define __TRIANGLE_H__


#include "Vec3.h"

struct Triangle
{
    int vertexIds[3];
	Vec3 normal;
	
    Triangle();
    Triangle(int vid1, int vid2, int vid3);
    Triangle(const Triangle &other);

    void setFirstVertexId(int vid);
    void setSecondVertexId(int vid);
    void setThirdVertexId(int vid);
};


#endif
