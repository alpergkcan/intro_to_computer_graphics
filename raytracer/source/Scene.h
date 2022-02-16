#ifndef RAYTRACER_SCENE_H
#define RAYTRACER_SCENE_H

#include "math.h"

#define MAXFLOAT 2e9

struct Object;
struct Ray{
	vec3f origin;
	vec3f vec;
	vec3f direction;

	float time;
	Object* hit;

	vec3f color;

	Ray() = default;
	Ray(const vec3f& point, const vec3f& out_ray){
		origin = point;
		vec = out_ray;
		direction = vec.normalized();
		color = {0,0,0};
		time = std::numeric_limits<float>::max();
		hit = NULL;
	}
	Ray& operator=(const Ray& rhs){
		origin = rhs.origin;
		vec = rhs.vec;
		direction = rhs.direction;
		hit = rhs.hit;
		color = rhs.color;
		return *this;
	}
};


struct pixel{
	Ray ray;
	color3i color;
	pixel& operator=(pixel& rhs){
		color = rhs.color;
		ray = rhs.ray;
		return *this;
	}
};

struct Image {
	float pixel_width, pixel_height;
	int   width,       height;

	std::string name;
	pixel** pixels;

	void exportPPM();
	
	~Image();
};

struct Camera{
	vec3f pos;

	vec3f gaze, up;
	vec3f u, v, w;
	
	vec4f near;
	float near_distance;
	vec3f topLeft, stepRight, stepDown;

	Image* img;

	void loadFromStream(std::stringstream& stream);

	
	~Camera();
};
class PointLight{
public:
	vec3f pos;
	vec3f intensity;

	void loadFromStream(std::stringstream& stream);

};
class Material {
public:
	bool isMirror;
	vec3f ambient;
	vec3f diffuse;
	vec3f specular;
	vec3f mirror;
	float phong_exponent;

};

struct Object {
	static std::vector<vec3f>& verts;
	
	int m_id;
	virtual void   intersect(Ray& ray)  = 0;
	virtual vec3f  normal(vec3f& point) = 0;
};

struct Sphere : public Object {
	void  intersect(Ray& ray) override;
	vec3f normal(vec3f& point) override;
	
	float radius;
	vec3f center;
};
struct Triangle : public Object {
	void  intersect(Ray& ray) override;
	vec3f normal(vec3f& point) override;
	
	int v1,
		v2,
		v3; // clockwise
	
	vec3f n;
};

class Scene {
public:
	color3i bg_color;
	float shadow_ray_epsilon;
	int max_recursion_depth;
	static void* sc;
	
	vec3f                   ambient_light;
	std::vector<PointLight*> point_lights;
	
	std::vector<Camera*>   cams;
	std::vector<Material*> mats;

	std::vector<vec3f>     vertices;  
	std::vector<Object*>   objects;
  
	void LoadScene(const char* path);
	int currentCam;

private:
	std::vector<Triangle*> triangles;
	std::vector<Sphere*>   spheres;
};

#endif
