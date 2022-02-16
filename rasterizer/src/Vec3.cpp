#include "Vec3.h"
#include <math.h>
#include <iomanip>

using namespace std;

Vec3::Vec3()
{
    this->x = 0.0;
    this->y = 0.0;
    this->z = 0.0;
    this->colorId = -1;
}

Vec3::Vec3(double x, double y, double z, int colorId)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->colorId = colorId;
}

Vec3::Vec3(const Vec3 &other)
{
    this->x = other.x;
    this->y = other.y;
    this->z = other.z;
    this->colorId = other.colorId;
}

double Vec3::getElementAt(int index)
{
    switch (index)
    {
    case 0:
        return this->x;

    case 1:
        return this->y;

    case 2:
        return this->z;

    default:
        return this->z;
    }
}

ostream& operator<<(ostream& os, const Vec3& v) {
    
    os << fixed << setprecision(6) << "[" << v.x << ", " << v.y << ", " << v.z << "]";

    return os;
}

double& Vec3::operator[](const int& index) {
    switch (index)
    {
    case 0:
        return this->x;

    case 1:
        return this->y;

    case 2:
        return this->z;

    default:
        return this->z;
    }
}

Vec3 Vec3::operator+(const Vec3& rhs){
	if(rhs.colorId)
		return Vec3(x+rhs.x, y+rhs.y, z+rhs.z, rhs.colorId);
	else if(colorId)
		return Vec3(x+rhs.x, y+rhs.y, z+rhs.z, colorId);
	else
		return Vec3(x+rhs.x, y+rhs.y, z+rhs.z, 0);
}

Vec3 Vec3::operator-(const Vec3& rhs){
	if(rhs.colorId) {
		return Vec3(x-rhs.x, y-rhs.y, z-rhs.z, rhs.colorId);
	}
	else if(colorId) {
		return Vec3(x-rhs.x, y-rhs.y, z-rhs.z, colorId);
	}
	else{
		return Vec3(x-rhs.x, y-rhs.y, z-rhs.z, 0);
	}
}

Vec3& Vec3::operator+=(const Vec3& rhs){
	this->x+=rhs.x;
	this->y+=rhs.y;
	this->z+=rhs.z;

	if(rhs.colorId)
		this->colorId = rhs.colorId;

	return *this;
}

Vec3& Vec3::operator-=(const Vec3& rhs){
	this->x-=rhs.x;
	this->y-=rhs.y;
	this->z-=rhs.z;

	if(rhs.colorId)
		this->colorId = rhs.colorId;

	return *this;
}

Vec3 Vec3::operator+(const double& n) {
	return Vec3(x+n, y+n, z+n, colorId);
}

Vec3 Vec3::operator-(const double& n) {
	return Vec3(x-n, y-n, z-n, colorId);
}

Vec3& Vec3::operator+=(const double& n) {
	this->x+=n;
	this->y+=n;
	this->z+=n;
	return *this;
}

Vec3& Vec3::operator-=(const double& n) {
	this->x-=n;
	this->y-=n;
	this->z-=n;
	return *this;
}

Vec3 Vec3::cross(const Vec3& v1, const Vec3& v2) {
	if(v2.colorId)
		return Vec3(v1.y*v2.z-v1.z*v2.y, v1.z*v2.x-v1.x*v2.z, v1.x*v2.y-v1.y*v2.x, v2.colorId);
	else if(v1.colorId)
		return Vec3(v1.y*v2.z-v1.z*v2.y, v1.z*v2.x-v1.x*v2.z, v1.x*v2.y-v1.y*v2.x, v1.colorId);
	else
		return Vec3(v1.y*v2.z-v1.z*v2.y, v1.z*v2.x-v1.x*v2.z, v1.x*v2.y-v1.y*v2.x, 0);
}

Vec3 Vec3::cross( Vec3* v1_, Vec3* v2_) {

	Vec3& v1 = *v1_;
	Vec3& v2 = *v2_;
	if(v2.colorId)
		return Vec3(v1.y*v2.z-v1.z*v2.y, v1.z*v2.x-v1.x*v2.z, v1.x*v2.y-v1.y*v2.x, v2.colorId);
	else if(v1.colorId)
		return Vec3(v1.y*v2.z-v1.z*v2.y, v1.z*v2.x-v1.x*v2.z, v1.x*v2.y-v1.y*v2.x, v1.colorId);
	else
		return Vec3(v1.y*v2.z-v1.z*v2.y, v1.z*v2.x-v1.x*v2.z, v1.x*v2.y-v1.y*v2.x, 0);

}

double Vec3::dot(const Vec3& v1, const Vec3& v2) {
	return (v1.x*v2.x + v1.y*v2.y + v1.z*v2.z);
}

Vec3 Vec3::mult(const Vec3& v1, const Vec3& v2) {
	if(v2.colorId)
		return Vec3(v1.x*v2.x, v1.y*v2.y, v1.z*v2.z, v2.colorId);
	else if(v1.colorId)
		return Vec3(v1.x*v2.x, v1.y*v2.y, v1.z*v2.z, v1.colorId);
	else
		return Vec3(v1.x*v2.x, v1.y*v2.y, v1.z*v2.z, 0         );
}

double Vec3::magnitude( const Vec3& v) {
	return sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}

Vec3 Vec3::normalized(const Vec3& v) {
	double n = 1/sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
	return Vec3(v.x*n, v.y*n, v.z*n, v.colorId);
}

double Vec3::magnitude() {
	return sqrt(x*x + y*y + z*z);
}

Vec3& Vec3::normalize(){
	double n = 1/sqrt(x*x + y*y + z*z);
	x*=n;
	y*=n;
	z*=n;
	return *this;;
}
