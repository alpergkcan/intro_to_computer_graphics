#include "Scene.h"

void Triangle::intersect(Ray& ray) {
	if(vec3f::dot(ray.vec , n) >0)
	   	return;

	vec3f& a = (verts)[v1];
	vec3f& b = (verts)[v2];
	vec3f& c = (verts)[v3];

	float
		A = a.x()-b.x(), D = a.x()-c.x(), G = ray.vec.x(),
		B = a.y()-b.y(), E = a.y()-c.y(), H = ray.vec.y(),
		C = a.z()-b.z(), F = a.z()-c.z(), I = ray.vec.z();

	float detA = A*(E*I - H*F)
		       + B*(G*F - D*I)
		       + C*(D*H - E*G);
	vec3f o = a - ray.origin;

	float beta  = (  o.x() * (E*I - H*F)
				   + o.y() * (G*F - D*I)
				   + o.z() * (D*H - E*G)  ) / detA;
	
	float gamma = (  A * (o.y()*I - H*o.z())
				   + B * (G*o.z() - o.x()*I)
				   + C * (o.x()*H - o.y()*G)  ) / detA;

	float tmpTime = (  A * (E*o.z() - o.y()*F)
					 + B * (o.x()*F - D*o.z())
					 + C * (D*o.y() - E*o.x()) ) / detA;

	bool  intersects = true;	
	intersects *= (0.0f <= tmpTime); 
	intersects *= ((beta + gamma) <= 1.0f);
	intersects *= (0.0f <= beta );
	intersects *= (0.0f <= gamma);
	
	if(intersects) {
			ray.time = tmpTime;
			ray.hit  = this;
	}
}

void Sphere::intersect(Ray &ray) {
	float b = vec3f::dot(ray.vec, ray.origin - center);
    float discriminant = (b*b) -
		ray.vec.sqrMagnitude() * ((ray.origin-center).sqrMagnitude()-radius*radius);

    if(discriminant < 0) {
        return;
    }
	else {
		float tmpTime = (-b - sqrt(discriminant))  / ray.vec.sqrMagnitude();
		if(0.0f <= tmpTime ){//&& tmpTime < ray.time){
			ray.time = tmpTime;
			ray.hit  = this;
		}
	}
}

vec3f Triangle::normal(vec3f& _) {
	return n;
}
vec3f Sphere::normal(vec3f& point) {
	return (point - center).normalized();
}

