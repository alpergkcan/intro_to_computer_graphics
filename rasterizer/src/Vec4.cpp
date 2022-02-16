
#include "Vec4.h"
#include <cmath>
#include <iomanip>

using namespace std;

Vec4::Vec4()
{
    this->x = 0.0;
    this->y = 0.0;
    this->z = 0.0;
    this->t = 0.0;
    this->colorId = -1;
}

Vec4::Vec4(double x, double y, double z, double t, int colorId)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->t = t;
    this->colorId = colorId;
}
Vec4::Vec4(const Vec4 &other)
{
    this->x = other.x;
    this->y = other.y;
    this->z = other.z;
    this->t = other.t;
    this->colorId = other.colorId;
}

double Vec4::getElementAt(int index)
{
    switch (index)
    {
    case 0:
        return this->x;

    case 1:
        return this->y;

    case 2:
        return this->z;

    case 3:
        return this->t;

    default:
        return this->t;
    }
}

ostream& operator<<(ostream& os, const Vec4& v) {
    
    os << fixed << setprecision(6) << "[" << v.x << ", " << v.y << ", " << v.z << ", " << v.t << "]";

    return os;
}


double &Vec4::operator[](const int& index){
	switch (index) {
    case 0:
        return this->x;

    case 1:
        return this->y;

    case 2:
        return this->z;

    case 3:
        return this->t;

    default:
        return this->t;
    }
}


Vec4 Vec4::operator+(const Vec4& rhs){
	if(rhs.colorId)
		return Vec4(x+rhs.x, y+rhs.y, z+rhs.z, t+rhs.t, rhs.colorId);
	else if(colorId)
		return Vec4(x+rhs.x, y+rhs.y, z+rhs.z, t+rhs.t, colorId);
	else
		return Vec4(x+rhs.x, y+rhs.y, z+rhs.z, t+rhs.t, 0);
}

Vec4 Vec4::operator-(const Vec4& rhs){
	if(rhs.colorId)
		return Vec4(x-rhs.x, y-rhs.y, z-rhs.z, t+rhs.t, rhs.colorId);
	else if(colorId)
		return Vec4(x-rhs.x, y-rhs.y, z-rhs.z, t+rhs.t, colorId);
	else
		return Vec4(x-rhs.x, y-rhs.y, z-rhs.z, t+rhs.t, 0);
}

Vec4& Vec4::operator+=(const Vec4& rhs){
	this->x+=rhs.x;
	this->y+=rhs.y;
	this->z+=rhs.z;
	this->t+=rhs.t;

	if(rhs.colorId)
		this->colorId = rhs.colorId;

	return *this;
}

Vec4& Vec4::operator-=(const Vec4& rhs){
	this->x-=rhs.x;
	this->y-=rhs.y;
	this->z-=rhs.z;
	this->t-=rhs.t;

	if(rhs.colorId)
		this->colorId = rhs.colorId;

	return *this;
}

Vec4 Vec4::operator+(const double& n) {
	return Vec4(x+n, y+n, z+n, t+n, colorId);
}

Vec4 Vec4::operator-(const double& n) {
	return Vec4(x-n, y-n, z-n, t-n, colorId);
}

Vec4& Vec4::operator+=(const double& n) {
	this->x+=n;
	this->y+=n;
	this->z+=n;
	this->t+=n;
	return *this;
}

Vec4& Vec4::operator-=(const double& n) {
	this->x-=n;
	this->y-=n;
	this->z-=n;
	this->t-=n;
	return *this;
}

double Vec4::dot(const Vec4& v1, const Vec4& v2) {
	return (v1.x*v2.x + v1.y*v2.y + v1.z*v2.z + v1.t*v2.t);
}

Vec4 Vec4::mult(const Vec4& v1, const Vec4& v2) {
	if(v2.colorId)
		return Vec4(v1.x*v2.x, v1.y*v2.y, v1.z*v2.z, v1.t*v2.t, v2.colorId);
	else if(v1.colorId)
		return Vec4(v1.x*v2.x, v1.y*v2.y, v1.z*v2.z, v1.t*v2.t, v1.colorId);
	else
		return Vec4(v1.x*v2.x, v1.y*v2.y, v1.z*v2.z, v1.t*v2.t, 0         );
}


double Vec4::magnitude( const Vec4& v) {
	return sqrt(v.x*v.x + v.y*v.y + v.z*v.z + v.t*v.t);
}

Vec4 Vec4::normalized(const Vec4& v) {
	double n = 1/sqrt(v.x*v.x + v.y*v.y + v.z*v.z + v.t*v.t);
	return Vec4(v.x*n, v.y*n, v.z*n, v.t*n, v.colorId);
}

double Vec4::magnitude() {
	return sqrt(x*x + y*y + z*z + t*t);
}

Vec4& Vec4::normalize(){
	double n = 1/sqrt(x*x + y*y + z*z + t*t);
	x*=n;
	y*=n;
	z*=n;
	t*=t;
	return *this;;
}

bool  Vec4::isOutsideBounds() {
	return ((x < -t) || (t < x) || (y < -t) || (t < y) || (z < -t) || (t < z));
}
