#ifndef __VEC3_H__
#define __VEC3_H__

#include <iostream>
using namespace std;

class Vec3
{
public:
    double x, y, z;
    int colorId;

    Vec3();
    Vec3(double x, double y, double z, int colorId);
    Vec3(const Vec3 &other);

    double getElementAt(int index);
	double &operator[](const int& index);

	Vec3 operator+(const Vec3& rhs);
	Vec3 operator-(const Vec3& rhs);
	Vec3& operator+=(const Vec3& rhs);
	Vec3& operator-=(const Vec3& rhs);

	Vec3 operator+(const double& rhs);
	Vec3 operator-(const double& rhs);
	Vec3& operator+=(const double& rhs);
	Vec3& operator-=(const double& rhs);

	Vec3& normalize();
	double magnitude();
	
	static Vec3 cross(const Vec3& v1, const Vec3& v2);
	static Vec3 cross(Vec3* v1, Vec3* v2);
	static Vec3 mult( const Vec3& v1, const Vec3& v2);
	static Vec3 normalized(const Vec3& v);
	
	static double dot(const Vec3& v1, const Vec3& v2);
	static double magnitude (const Vec3& v);

    friend std::ostream& operator<<(std::ostream& os, const Vec3& v);
};


#endif
