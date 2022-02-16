#ifndef __VEC4_H__
#define __VEC4_H__

#include <iostream>
using namespace std;

class Vec4
{
public:
    double x, y, z, t;
    int colorId;


    Vec4();
    Vec4(double x, double y, double z, double t, int colorId);
    Vec4(const Vec4 &other);
    
    double getElementAt(int index);
	double &operator[](const int& index);

	Vec4 operator+(const Vec4& rhs);
	Vec4 operator-(const Vec4& rhs);
	Vec4& operator+=(const Vec4& rhs);
	Vec4& operator-=(const Vec4& rhs);

	Vec4 operator+(const double& rhs);
	Vec4 operator-(const double& rhs);
	Vec4& operator+=(const double& rhs);
	Vec4& operator-=(const double& rhs);

	
	static Vec4 cross(const Vec4& v1, const Vec4& v2);
	static double dot(const Vec4& v1, const Vec4& v2);
	static Vec4 mult( const Vec4& v1, const Vec4& v2);

	Vec4& normalize();
	double magnitude();
	static double magnitude(const Vec4& v);
	static Vec4 normalized( const Vec4& v) ;
	bool isOutsideBounds();
	
    friend std::ostream& operator<<(std::ostream& os, const Vec4& v);
};

#endif
