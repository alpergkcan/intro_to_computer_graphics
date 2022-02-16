#include "Scene.h"
#include "tinyxml2.h"

#include <string>
#include <sstream>

std::vector<vec3f> a = {vec3f{0,0,0}};
std::vector<vec3f>& Object::verts = a;
void* Scene::sc;

void Scene::LoadScene(const char* path) {
	using namespace tinyxml2;

	XMLDocument xml;
	std::stringstream stream;

  
	XMLError error = xml.LoadFile(path);
	if(error)
		throw std::runtime_error("Error On Scene Load: xml.LoadFile(path)");
	XMLNode* root = xml.FirstChild();
	if(!root)
		throw std::runtime_error("Error On Scene Load: xml.Firstchild()");
	XMLElement* element = root->FirstChildElement("BackgroundColor");
	if (element)
		stream << element->GetText() << std::endl;
	else
		stream << "0 0 0" << std::endl;

	bg_color.loadFromStream(stream);

	element = root->FirstChildElement("MaxRecursionDepth");
	if(element)
		stream << element->GetText() << std::endl;
	else
		stream << "0" << std::endl;

	stream >> max_recursion_depth;

	element = root->FirstChildElement("ShadowRayEpsilon");
	if(element)
		stream << element->GetText() << std::endl;
	else
		stream << "0.001" << std::endl;
	stream >> shadow_ray_epsilon;

		element = root->FirstChildElement("Cameras");
	element = element->FirstChildElement("Camera");
	Camera* cam;
	XMLElement* child;
	while(element) {
		cam = new Camera;
		child = element->FirstChildElement("Position");
		stream << child->GetText() << std::endl;
		child = element->FirstChildElement("Gaze");
		stream << child->GetText() << std::endl;
		child = element->FirstChildElement("Up");
		stream << child->GetText() << std::endl;
		child = element->FirstChildElement("NearPlane");
		stream << child->GetText() << std::endl;
		child = element->FirstChildElement("NearDistance");
		stream << child->GetText() << std::endl;
		child = element->FirstChildElement("ImageResolution");
		stream << child->GetText() << std::endl;
		child = element->FirstChildElement("ImageName");
		stream << child->GetText() << std::endl;

		cam->loadFromStream(stream);
		
		cams.push_back(cam);
		element = element->NextSiblingElement("Camera");
	}
	currentCam = 0;

	element = root->FirstChildElement("Lights");
	child = element->FirstChildElement("AmbientLight");
	if(child){
		stream << child->GetText() <<std::endl;
		ambient_light.loadFromStream(stream);
	}
	else
		throw std::runtime_error("No AmbientLight on Scene");

	element = element->FirstChildElement("PointLight");
	PointLight* pLight;
	while(element){
		pLight = new PointLight;
		child = element->FirstChildElement("Position");
		stream << child->GetText() << std::endl;
		child = element->FirstChildElement("Intensity");
		stream << child->GetText() << std::endl;

		pLight->pos.loadFromStream(stream);
		pLight->intensity.loadFromStream(stream);
		
		point_lights.push_back(pLight);
		element = element->NextSiblingElement("PointLight");
	}


	element = root->FirstChildElement("Materials");
	element = element->FirstChildElement("Material");
	Material* mat;
	while(element){
		mat = new Material;

		mat->isMirror = (element->Attribute("type", "mirror") != NULL );

		child = element->FirstChildElement("AmbientReflectance");
		stream << child->GetText() << std::endl;
		child = element->FirstChildElement("DiffuseReflectance");
		stream << child->GetText() << std::endl;
		child = element->FirstChildElement("SpecularReflectance");
		stream << child->GetText() << std::endl;
		child = element->FirstChildElement("MirrorReflectance");
		stream << child->GetText() << std::endl;
		child = element->FirstChildElement("PhongExponent");
		stream << child->GetText() << std::endl;

		mat->ambient.loadFromStream(stream);
		mat->diffuse.loadFromStream(stream);
		mat->specular.loadFromStream(stream);
		mat->mirror.loadFromStream(stream);
		stream >> mat->phong_exponent;

		mats.push_back(mat);
		element = element->NextSiblingElement("Material");
	}
	stream.clear();

	element = root->FirstChildElement("VertexData");
	stream << element->GetText() << std::endl;

	vec3f* vert;
	while( true ){
		vert = new vec3f;
		vert->loadFromStream(stream);
		if(stream.eof())
			break;
		vertices.push_back(*vert);
	}
	stream.clear();
	Object::verts = vertices;

	element = root->FirstChildElement("Objects");
	element = element->FirstChildElement("Mesh");
	while(element){
		child = element->FirstChildElement("Material");
		stream << child->GetText()<< std::endl;
		int m_id;
		stream >> m_id;
		m_id--;

		child = element->FirstChildElement("Faces");
		stream << child->GetText() << std::endl;
		Triangle* tri;
		while( true ){
			tri = new Triangle;
			stream >> tri->v1 >> tri->v2 >> tri->v3;
			if( stream.eof())
				break;
			tri->v1--;
			tri->v2--;
			tri->v3--;
	  
			tri->n = vec3f::cross(
				vertices[tri->v2] - vertices[tri->v1],
				vertices[tri->v3] - vertices[tri->v1]).normalized();

			tri->m_id = m_id;
			triangles.push_back(tri);
			objects.push_back(tri);
		}
		stream.clear();
		element = element->NextSiblingElement("Mesh");
	}
	element = root->FirstChildElement("Objects");
	element = root->FirstChildElement("Triangle");
	Triangle* tri;
	while(element){
		tri = new Triangle;
		child = element->FirstChildElement("Material");
		stream << child->GetText() << std::endl;
		stream >> tri->m_id;
		tri->m_id--;

		child = element->FirstChildElement("Indices");
		stream << child->GetText() << std::endl;
		stream >> tri->v1 >> tri->v2 >> tri->v3;
		tri->v1--;
		tri->v2--;
		tri->v3--;

		tri->n = vec3f::cross(
			vertices[tri->v2] - vertices[tri->v1],
			vertices[tri->v3] - vertices[tri->v1]).normalized();
	
		triangles.push_back(tri);
		objects.push_back(tri);
		element = element->NextSiblingElement("Triangle");
	}

	element = root->FirstChildElement("Objects");
	element = element->FirstChildElement("Sphere");
	Sphere* sp;
	while(element){
		sp = new Sphere;

		child = element->FirstChildElement("Material");
		stream << child->GetText() << std::endl;
		stream >> sp->m_id;
		sp->m_id--;

		child = element->FirstChildElement("Center");
		stream << child->GetText() << std::endl;
		int center_vertex_id;
		stream >> center_vertex_id;
		center_vertex_id--;

		sp->center = vertices[center_vertex_id];

		child = element->FirstChildElement("Radius");
		stream << child->GetText() << std::endl;
		stream >> sp->radius;

		spheres.push_back(sp);
		objects.push_back(sp);
		element = element->NextSiblingElement("Sphere");
	}
	Scene::sc = this;
}

void Camera::loadFromStream(std::stringstream &stream) {
	  pos.loadFromStream(stream);
	  gaze.loadFromStream(stream);
	  up.loadFromStream(stream);
	  near.loadFromStream(stream);
	  stream >> near_distance;

	  v = up.normalized();
	  w = (-1*gaze).normalized();
	  u = vec3f::cross(v, w).normalized();
	  
	  img = new Image();
	  stream >> img->width >> img->height;
	  stream >> img->name;

	  vec3f center =  pos + near_distance * gaze;

	  float near_l = near.x();
	  float near_r = near.y();
	  float near_b = near.z();
	  float near_t = near.w();

	  float world_width  = near_r - near_l;
	  float world_height = near_t - near_b;

	  img->pixel_width  = world_width  / img->width;
	  img->pixel_height = world_height / img->height;

	  topLeft = center  + u * near_l + up * near_t;
	  topLeft   = topLeft + u * img->pixel_width * 0.5f + up * img->pixel_height * -0.5f;
	  
	  stepRight = u * img->pixel_width;
	  stepDown  = -1 * up    * img->pixel_height;
}

void Image::exportPPM() {
	FILE *outfile;

    if ((outfile = fopen(name.c_str(), "w")) == NULL) 
        throw std::runtime_error("Error: The ppm file cannot be opened for writing.");

    (void) fprintf(outfile, "P3\n%d %d\n255\n", width, height);

	for(int i = 0; i < this->height; i++){
	  for(int j = 0; j < this->width; j++) {
		   color3i& color = this->pixels[i][j].color;
			if (j == width - 1) 
 				fprintf(outfile, "%hu %hu %hu\n", color.x(), color.y(), color.z());
			else
				fprintf(outfile, "%hu %hu %hu  ", color.x(), color.y(), color.z());
        }
    }
	fclose(outfile);
}
Camera::~Camera() {
	delete img;
}

Image::~Image() {
	for( int i = 0; i < height; i++)
		delete [] pixels[i];
	delete [] pixels;
}

