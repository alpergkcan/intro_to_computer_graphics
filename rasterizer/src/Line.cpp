#include "Line.h"

Line::Line(Vec3& origin, Vec3& direction, double length){
	this->origin = origin;
	this->direction = direction;
	this->length = length;
}
Line::Line(Vec3& from, Vec3& to) {
	Vec3 ray = to - from;
	ray.colorId = to.colorId;
	this->length = ray.magnitude();
	this->direction = Vec3::normalized(ray);
	this->origin = from;
}




