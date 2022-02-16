#include "math.h"
#include <cmath>

// cout
std::ostream& operator<<( std::ostream& oss,  vec3f& rhs){
	return oss << rhs.toString();
	}
std::ostream& operator<<( std::ostream& oss,  vec3d& rhs){
	return oss << rhs.toString();
	}
std::ostream& operator<<( std::ostream& oss,  vec3i& rhs){
	return oss << rhs.toString();
	}


std::ostream& operator<<( std::ostream& oss,  vec4f& rhs){
	return oss << rhs.toString();
	}
std::ostream& operator<<( std::ostream& oss,  vec4d& rhs){
	return oss << rhs.toString();
	}
std::ostream& operator<<( std::ostream& oss,  vec4i& rhs){
	return oss << rhs.toString();
	}


vec3f operator*(float lhs,  const vec3f& rhs){
  return rhs * lhs;
}
vec4f operator*(float lhs,  const vec4f& rhs){
  return rhs * lhs;
}

vec3f operator+(float lhs, const vec3f& rhs){
    vec3f tmp(lhs);
	return tmp + rhs;
}
vec4f operator+(float lhs, const vec4f& rhs){
    vec4f tmp(lhs);
	return tmp + rhs;
}

vec3f operator-(float lhs, const vec3f& rhs){
  vec3f tmp(lhs);
  return tmp - rhs;
}
vec4f operator-(float lhs, const vec4f& rhs){
  vec4f tmp(lhs);
  return tmp - rhs;
}



vec3f operator*(float lhs, const vec3i& rhs) {
	return lhs * vec3f(rhs.x(), rhs.y(), rhs.z());
}
vec3f operator+(float lhs, const vec3i& rhs) {
	return lhs + vec3f(rhs.x(), rhs.y(), rhs.z());
}
vec3f operator-(float lhs, const vec3i& rhs) {
	return lhs - vec3f(rhs.x(), rhs.y(), rhs.z());
}


color3i operator*(float lhs, const color3i& rhs) {
	vec3f tmp = lhs * vec3f(rhs.x(), rhs.y(), rhs.z());
	return {(unsigned short)floor(tmp.x()), (unsigned short)floor(tmp.y()), (unsigned short)floor(tmp.z())};
}
color3i operator+(float lhs, const color3i& rhs) {
	vec3f tmp = lhs + vec3f(rhs.x(), rhs.y(), rhs.z());
	return {(unsigned short)floor(tmp.x()), (unsigned short)floor(tmp.y()), (unsigned short)floor(tmp.z())};
}
color3i operator-(float lhs, const color3i& rhs) {
	vec3f tmp = lhs - vec3f(rhs.x(), rhs.y(), rhs.z());
	return {(unsigned short)floor(tmp.x()), (unsigned short)floor(tmp.y()), (unsigned short)floor(tmp.z())};
}
