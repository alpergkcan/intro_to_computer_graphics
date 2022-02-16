
#include "Scene.h"
#include <thread>
#include <time.h>
#include <cmath>
#include <algorithm>
#include <climits>

#define EPSILON (std::numeric_limits<float>::epsilon())

void rt_threaded(Scene* s, int partition);
void ray_tracer(Ray& ray, Scene& scene, int depth) ;

int main(int _, char** args){
	Scene *scene = new Scene();
	scene->LoadScene(args[1]);

	int camCount = scene->cams.size();
	for(int c = 0; c<camCount ; c++ ) {
		Camera* cam = scene->cams[c];
		scene->currentCam = c;

		cam->img->pixels = new pixel*[cam->img->height]; // row
		for(int i = 0; i<cam->img->height; i++){
			cam->img->pixels[i] = new pixel[cam->img->width]();
			for( int j = 0;  j < cam->img->width; j++) { // height
				cam->img->pixels[i][j].ray =
					{cam->pos, (cam->topLeft + cam->stepDown * i + cam->stepRight * j) - cam->pos};
			}
		}

		
		std::thread thread1(rt_threaded, scene, 0);
		std::thread thread2(rt_threaded, scene, 1);
		std::thread thread3(rt_threaded, scene, 2);
		std::thread thread4(rt_threaded, scene, 3);
		thread1.join();
		thread2.join();
		thread3.join();
		thread4.join();

		cam->img->exportPPM();
		for(int i = 0; i<cam->img->height; i++)
			delete [] cam->img->pixels[i];
		delete [] cam->img->pixels;
	}

	return 0;
};

	
void rt_threaded(Scene * scene, int partition) {

	Camera* cam  = (Camera*)(scene->cams[scene->currentCam]);

	if(3 < partition || partition < 0)
		throw std::runtime_error("wrong threding with partition: " + std::to_string(partition));

	Image* im = cam->img;

	int offset_x = 0 + ((partition % 2) == 1) * (im->width/2);
	int offset_y = 0 + ( partition >= 2     ) * (im->height/2);
	int end_x = (cam->img->width  / 2) + offset_x;
	int end_y = (cam->img->height / 2) + offset_y;

	for(int i = offset_y; i < end_y; i++) {
		for(int j = offset_x; j < end_x; j++) {
			pixel& p = im->pixels[i][j];
			p.ray.origin = cam->pos;

			ray_tracer(p.ray, *scene, 0);

			p.color = p.ray.color.round();
		}
	}
	return;
}

void ray_tracer(Ray& ray, Scene& scene, int depth) {
	if(depth > scene.max_recursion_depth)
		return;

			
	ray.time = MAXFLOAT;
	ray.hit = NULL;

	float timeMin = MAXFLOAT;
	Object* hit = NULL;
	for(Object* t : scene.objects) {
		t->intersect(ray);
		if( ray.time < timeMin){
			timeMin = ray.time;
			hit = (Object*)ray.hit;
		}
	}

	if(hit == NULL){
		if(depth == 0 ) {
			for(int i = 0; i < 3; i ++)
				ray.color.arr[i] = (float)(scene.bg_color.arr[i]);
		}
		else
			ray.color = {0,0,0};
		return;
	}

	Material* mat = scene.mats[hit->m_id];

	vec3f point = ray.origin + ray.vec * timeMin;
	vec3f normal = hit->normal(point);
	vec3f shadow_point = point + normal * scene.shadow_ray_epsilon;
		
	// AMBIENT
	ray.color = mat->ambient * scene.ambient_light;

	for(PointLight* pl : scene.point_lights) {
		vec3f light_ray = pl->pos - point;

		vec3f tt = pl->pos - shadow_point;
		Ray shadow_ray(shadow_point, tt);

		bool flag = false;
		for(Object* t: scene.objects){
			t->intersect(shadow_ray);
			if(shadow_ray.hit != NULL &&
			   shadow_ray.time <= 1.0f) {
				flag = true;
				break;
			}
		}
		if(flag)
			continue;

		// DIFFUSE
		vec3f irradiance = (pl->intensity / light_ray.sqrMagnitude());
		ray.color = ray.color + mat->diffuse *
			std::max(0.0f, vec3f::dot(light_ray.normalized(), normal)) * irradiance;

		// SPECULAR
		float cosP = vec3f::dot(normal, ((ray.origin-point)+light_ray).normalized());
		if(0.0f<= cosP) {
			if( cosP <= EPSILON)
				ray.color = ray.color + mat->specular * irradiance;
			else 
				ray.color = ray.color + mat->specular
					* std::pow(cosP, mat->phong_exponent) * irradiance;
		}
	}
	//REFLACTANCE
	if(mat->isMirror) {
		vec3f&& w0 = (ray.origin - point).normalized();
		vec3f&& wr = (-1 * w0 + 2 * normal * vec3f::dot(normal, w0)).normalized();
		
		Ray wRay(shadow_point, wr);
		ray_tracer(wRay, scene, depth+1);
		ray.color = ray.color + mat->mirror * wRay.color;
	}
		
	return;
}

