#ifndef HW2_LINE_H
#define HW2_LINE_H

#include "Vec3.h"

struct Line{
	Vec3 origin;
	Vec3 direction;
	double length;

	Line(Vec3& origin, Vec3& direction, double length);
	Line(Vec3& from, Vec3& to);
};

#endif
