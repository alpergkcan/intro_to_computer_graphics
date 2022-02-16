#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <cmath>
#include <climits>

#include "Scene.h"
#include "Camera.h"
#include "Color.h"
#include "Mesh.h"
#include "Rotation.h"
#include "Scaling.h"
#include "Translation.h"
#include "Triangle.h"
#include "Vec3.h"
#include "Line.h"
#include "tinyxml2.h"
#include "Helpers.h"

#define f01(x, y) ((y0-y1)*x + (x1-x0)*y + x0*y1 - y0*x1)
#define f12(x, y) ((y1-y2)*x + (x2-x1)*y + x1*y2 - y1*x2)
#define f20(x, y) ((y2-y0)*x + (x0-x2)*y + x2*y0 - y2*x0)

using namespace tinyxml2;
using namespace      std;

/*
	Transformations, clipping, culling, rasterization are done here.
	You may define helper functions.

	!! @TODO: lacking parts: clipping, triangle rasterizer, viewport
*/
void Scene::forwardRenderingPipeline(Camera *camera) {
	// TODO: Implement this function.

	Matrix4 view;
	view = camera->GetView();

	Matrix4 projection; 
	projection = camera->GetProjection();

	Matrix4 viewport;
	viewport   = camera->GetViewport();


	Matrix4 vp;

	vp = multiplyMatrixWithMatrix(projection, view);

	for( Mesh* m : meshes) {
		m->model = getIdentityMatrix();

		for(int i = 0; i < m->numberOfTransformations ; i++) {
			m->model = multiplyMatrixWithMatrix
				(
					CreateTransformationMatrix(m->transformationIds[i]-1, m->transformationTypes[i] ),
					m->model
				);
		}
		
		Matrix4 mv;		
		Matrix4 mvp;		
		if(cullingEnabled)
			mv = multiplyMatrixWithMatrix( view, m->model );
		else
			mvp = multiplyMatrixWithMatrix( vp, m->model );

		for(Triangle& t : m->triangles) {
			Vec3* v1 = vertices[ t.vertexIds[0] ];
			Vec3* v2 = vertices[ t.vertexIds[1] ];
			Vec3* v3 = vertices[ t.vertexIds[2] ];
				
			Vec4 res_v1, res_v2,res_v3;
			if( cullingEnabled ) {
				res_v1 = multiplyMatrixWithVec4(mv, Vec4(v1->x,v1->y,v1->z, 1, v1->colorId));
				res_v2 = multiplyMatrixWithVec4(mv, Vec4(v2->x,v2->y,v2->z, 1, v2->colorId));
				res_v3 = multiplyMatrixWithVec4(mv,	Vec4(v3->x,v3->y,v3->z, 1, v3->colorId));

				if(isCulledOff(res_v1, res_v2, res_v3, camera))
					continue;
				
				res_v1 = multiplyMatrixWithVec4(projection, res_v1);
				res_v2 = multiplyMatrixWithVec4(projection, res_v2);
				res_v3 = multiplyMatrixWithVec4(projection, res_v3);
			}
			else {
				res_v1 = multiplyMatrixWithVec4(mvp, Vec4(v1->x,v1->y,v1->z, 1, v1->colorId));
				res_v2 = multiplyMatrixWithVec4(mvp, Vec4(v2->x,v2->y,v2->z, 1, v2->colorId));
				res_v3 = multiplyMatrixWithVec4(mvp, Vec4(v3->x,v3->y,v3->z, 1, v3->colorId));
			}

			// if( m->type == 0 ) {
			// 	if( res_v1.isOutsideBounds() &&
			// 		res_v2.isOutsideBounds() &&
			// 		res_v3.isOutsideBounds()    )
			// 		continue;
			// }
			// else {
			// 	if( res_v1.isOutsideBounds() ||
			// 		res_v2.isOutsideBounds() ||
			// 		res_v3.isOutsideBounds()    )
			// 		continue;
			// }
			
			double div;
			double mag = 1;
			div = 1/(res_v1.t * mag);
			res_v1 = multiplyVec4WithScalar(res_v1, div);

			div = 1/(res_v2.t * mag);
			res_v2 = multiplyVec4WithScalar(res_v2, div);
				
			div = 1/(res_v3.t * mag);
			res_v3 = multiplyVec4WithScalar(res_v3, div);

			res_v1 = multiplyMatrixWithVec4(viewport, res_v1);
			res_v2 = multiplyMatrixWithVec4(viewport, res_v2);
			res_v3 = multiplyMatrixWithVec4(viewport, res_v3);

			Rasterize(res_v1, res_v2, res_v3, (bool)m->type);
		}
	}
	
}

void Scene::Rasterize(Vec4& v1, Vec4& v2, Vec4& v3, bool isSolid){
	if( isSolid ){ // solid
		TriangleRasterizer(v1, v2, v3);
	}
	else { // wireframe
		LineRasterizer(v1, v2);
		LineRasterizer(v2, v3);
		LineRasterizer(v3, v1);
	}
}

void Scene::LineRasterizer( Vec4& p1, Vec4& p2) {
	double x0 = p1.x, y0 = p1.y,
		   x1 = p2.x, y1 = p2.y;
	int cid0 = p1.colorId, cid1 = p2.colorId;
	
	double flr;
	// floor/ceil to the closest x.5
	flr = floor(x0);
	x0  = flr + ((x0 - flr >= 0.5) ? 0.5 : -0.5);

	flr = floor(x1);
	x1  = flr + ((x1 - flr >= 0.5) ? 0.5 : -0.5);

	flr = floor(y0);
	y0  = flr + ((y0 - flr >= 0.5) ? 0.5 : -0.5);

	flr = floor(y1);
	y1  = flr + ((y1 - flr >= 0.5) ? 0.5 : -0.5);
	
	// fix point ordering when x1 is less then x0 (i.e. line going to its left)
	double hori_dist = x1 - x0;
	if( hori_dist < 0 ) {  
		double tmp;
		tmp = x1;
		x1 = x0;
		x0 = tmp;

		tmp = y1;
		y1 = y0;
		y0 = tmp;

		tmp = cid0;
		cid0 = cid1;
		cid1 = (int)tmp;

		hori_dist *= -1;
	}
	else if (hori_dist == 0) { // @fix uncomment when paint function is written
		// int dif = ((y1 - y0 >= 0) ? 1 : -1);
		// for(double y = y0; y < y1 || ((y1 - 0.1) < y && y < (y1 + 0.1)); y += dif)
		//     paint(x0, y, SOME_COLOR) // @nocheckin
	}


	double vert_dist    = y1 - y0;
	bool   slope_over_1 = abs(vert_dist / hori_dist) > 1;
	if( slope_over_1 ) { // slope over 1, so interchange x and y
		double tmp;
		tmp = x0;
		x0 = y0;
		y0 = tmp;

		tmp = x1;
		x1 = y1;
		y1 = tmp;
	}


	// the ultimate midpoint line rendering algorithm to rule them all @atleastihopeso
	int sy = (y1 > y0) ? 1 : -1 ;
	int sx = (x1 > x0) ? 1 : -1 ;

	double dx = abs(x1-x0);
	double dy = -abs(y1-y0);

	double err = dx + dy;
	double e2;

	Color RED(255, 0, 0);
	Color* c1 = this->colorsOfVertices[cid0];
	Color* c2 = this->colorsOfVertices[cid1];
	double distance = sqrt(pow(x1 - x0, 2) + pow(y1 - y0, 2));
	Color  c_dif = Color( (c2->r - c1->r) / distance,
						  (c2->g - c1->g) / distance,
						  (c2->b - c1->b) / distance );

	while( true ) {
		double cur_dist = sqrt( pow( x1 - x0 , 2) + pow( y1 - y0, 2) );		
		Color  f_color  = Color( c1->r + cur_dist * c_dif.r,
								 c1->g + cur_dist * c_dif.g,
								 c1->b + cur_dist * c_dif.b );

		// if(x0 > image[0].size() || y0 > image.size())
		// 	break;
		// @fix uncomment when fuction paint is written
		if( slope_over_1 ) // @nocheckin
			paint(y0, x0, f_color );
		else 
			paint( x0, y0, f_color );


		if( x0 == x1 && y0 == y1 )
			break;
		e2 = 2 * err;
		if ( e2 >= dy ) {
			err += dy;
			x0  += sx;
		}
		if ( e2 <= dx ) {
			err += dx;
			y0  += sy;
		}
		
	}
}


void Scene::TriangleRasterizer(Vec4& v1, Vec4& v2, Vec4& v3) {
	double min_x = MAXFLOAT, max_x = -MAXFLOAT,
	       min_y = MAXFLOAT, max_y = -MAXFLOAT;
	double x0, x1, x2,
		   y0, y1, y2;
	double flr;

	// floor/ceil to the closest x.5
	flr = floor(v1.x);
	x0  = flr +((v1.x - flr >= 0.5) ? 0.5 : -0.5);
	if( x0 < min_x ) min_x = x0;
	if( x0 > max_x ) max_x = x0;
	
	
	flr = floor(v2.x);
	x1  = flr +((v2.x - flr >= 0.5) ? 0.5 : -0.5);
	if( x1 < min_x ) min_x = x1;
	if( x1 > max_x ) max_x = x1;

	flr = floor(v3.x);
	x2  = flr +((v3.x - flr >= 0.5) ? 0.5 : -0.5);
	if( x2 < min_x ) min_x = x2;
	if( x2 > max_x ) max_x = x2;
	
	flr = floor(v1.y);
	y0  = flr +((v1.y - flr >= 0.5) ? 0.5 : -0.5);
	if( y0 < min_y ) min_y = y0;
	if( y0 > max_y ) max_y = y0;

	flr = floor(v2.y);
	y1  = flr +((v2.y - flr >= 0.5) ? 0.5 : -0.5);
	if( y1 < min_y ) min_y = y1;
	if( y1 > max_y ) max_y = y1;

	flr = floor(v3.y);
	y2  = flr +((v3.y - flr >= 0.5) ? 0.5 : -0.5);
	if( y2 < min_y ) min_y = y2;
	if( y2 > max_y ) max_y = y2;

	Color* c0 = colorsOfVertices[v1.colorId];
	Color* c1 = colorsOfVertices[v2.colorId];
	Color* c2 = colorsOfVertices[v3.colorId];

    // find barycentric values
	double alpha, beta, gamma;
	double f_alpha, f_beta, f_gamma;
	f_alpha = f12(x0, y0);
	f_beta  = f20(x1, y1);
	f_gamma = f01(x2, y2);

	double distt;
	double maxdistt;
	Vec4 *dv1, *dv2;
	if(f_alpha == 0 || f_beta == 0 || f_gamma == 0){
		return;
		maxdistt = distt = (v1 - v2).magnitude();
		dv1 = &v1, dv2 = &v2;

		distt = (v2-v3).magnitude();
		if(maxdistt < distt) {
			maxdistt = distt;
			dv1 = &v2;
			dv2 = &v3;
		}

		distt = (v3-v1).magnitude();
		if(maxdistt < distt) {
			maxdistt = distt;
			dv1 = &v1;
			dv2 = &v3;
		}

		for( double y = min_y; y <= max_y; y +=1 ) {
			for(double x = min_x; x <= max_x; x += 1 ) {
				double S = 0;
				S += sqrt( pow(x - dv1->x,2) + pow(y - dv1->y, 2) );
				S += sqrt( pow(x - dv2->x,2) + pow(y - dv2->y, 2) );				
				if ( maxdistt >= S) {
					if( f_alpha != 0.0 )
						alpha = f12(x, y) / f_alpha;
					else
						alpha = f12(x, y) / EPSILON;

					if( f_beta != 0.0 )
						beta  = f20(x, y) / f_beta;
					else
						beta  = f20(x, y) / EPSILON;

					if( f_gamma != 0.0 )
						gamma = f01(x, y) / f_gamma;
					else
						gamma = f01(x, y) / EPSILON;
					
					Color f_color;
					f_color.r = alpha * c0->r + beta * c1->r + gamma * c2->r;
					f_color.g = alpha * c0->g + beta * c1->g + gamma * c2->g;
					f_color.b = alpha * c0->b + beta * c1->b + gamma * c2->b;
					paint(x, y, f_color);
				}
			}
		}
	}
	
	for( double y = min_y; y <= max_y; y +=1 ) {
		for(double x = min_x; x <= max_x; x += 1 ) {
			alpha = f12(x, y) / f_alpha;
			beta  = f20(x, y) / f_beta;
			gamma = f01(x, y) / f_gamma;

			if( (alpha >= 0 && beta >= 0  &&  gamma >= 0) &&
				(alpha >  0 || f_alpha * f12(-1, -1) > 0) &&
				(beta  >  0 || f_beta  * f20(-1, -1) > 0) &&
				(gamma >  0 || f_gamma * f01(-1, -1) > 0)  ) {

				Color f_color;
				f_color.r = alpha * c0->r + beta * c1->r + gamma * c2->r;
				f_color.g = alpha * c0->g + beta * c1->g + gamma * c2->g;
				f_color.b = alpha * c0->b + beta * c1->b + gamma * c2->b;
				paint(x, y, f_color);
			}
		}
	}
	
	return;
}


void Scene::paint(double x_loc, double y_loc, Color color_rgb) {
	if(0 > x_loc || x_loc > image.size() ||
	   y_loc < 0 || y_loc > image[0].size()      )
		return;
	
    this->image[x_loc][y_loc] = color_rgb;
}

/*
	Parses XML file
*/
Scene::Scene(const char *xmlPath)
{
	const char *str;
	XMLDocument xmlDoc;
	XMLElement *pElement;

	xmlDoc.LoadFile(xmlPath);

	XMLNode *pRoot = xmlDoc.FirstChild();

	// read background color
	pElement = pRoot->FirstChildElement("BackgroundColor");
	str = pElement->GetText();
	sscanf(str, "%lf %lf %lf", &backgroundColor.r, &backgroundColor.g, &backgroundColor.b);

	// read culling
	pElement = pRoot->FirstChildElement("Culling");
	if (pElement != NULL) {
		str = pElement->GetText();
		
		if (strcmp(str, "enabled") == 0)
			cullingEnabled = true;
		else
			cullingEnabled = false;
	}

	// read cameras
	pElement = pRoot->FirstChildElement("Cameras");
	XMLElement *pCamera = pElement->FirstChildElement("Camera");
	XMLElement *camElement;
	while (pCamera != NULL)
	{
		Camera *cam = new Camera();

		pCamera->QueryIntAttribute("id", &cam->cameraId);

		// read projection type
		str = pCamera->Attribute("type");

		if (strcmp(str, "orthographic") == 0)
			cam->projectionType = 0;
		else
			cam->projectionType = 1;
		
		camElement = pCamera->FirstChildElement("Position");
		str = camElement->GetText();
		sscanf(str, "%lf %lf %lf", &cam->pos.x, &cam->pos.y, &cam->pos.z);

		camElement = pCamera->FirstChildElement("Gaze");
		str = camElement->GetText();
		sscanf(str, "%lf %lf %lf", &cam->gaze.x, &cam->gaze.y, &cam->gaze.z);

		camElement = pCamera->FirstChildElement("Up");
		str = camElement->GetText();
		sscanf(str, "%lf %lf %lf", &cam->v.x, &cam->v.y, &cam->v.z);

		cam->gaze = normalizeVec3(cam->gaze);
		cam->u = crossProductVec3(cam->gaze, cam->v);
		cam->u = normalizeVec3(cam->u);

		cam->w = inverseVec3(cam->gaze);
		cam->v = crossProductVec3(cam->u, cam->gaze);
		cam->v = normalizeVec3(cam->v);

		camElement = pCamera->FirstChildElement("ImagePlane");
		str = camElement->GetText();
		sscanf(str, "%lf %lf %lf %lf %lf %lf %d %d",
			   &cam->left,   &cam->right,
			   &cam->bottom, &cam->top,
			   &cam->near,   &cam->far,
			   &cam->horRes, &cam->verRes);

		camElement = pCamera->FirstChildElement("OutputName");
		str = camElement->GetText();
		cam->outputFileName = string(str);

		cameras.push_back(cam);

		pCamera = pCamera->NextSiblingElement("Camera");
	}

	// read vertices
	pElement = pRoot->FirstChildElement("Vertices");
	XMLElement *pVertex = pElement->FirstChildElement("Vertex");
	int vertexId = 1;

	while (pVertex != NULL)
	{
		Vec3 *vertex = new Vec3();
		Color *color = new Color();

		vertex->colorId = vertexId-1;

		str = pVertex->Attribute("position");
		sscanf(str, "%lf %lf %lf", &vertex->x, &vertex->y, &vertex->z);

		str = pVertex->Attribute("color");
		sscanf(str, "%lf %lf %lf", &color->r, &color->g, &color->b);

		vertices.push_back(vertex);
		colorsOfVertices.push_back(color);

		pVertex = pVertex->NextSiblingElement("Vertex");

		vertexId++;
	}
	int id = 0;
	// for(auto& c : colorsOfVertices) {
	// 	std::cout << id << " - " << *c << std::endl;
	// 	id++;
	// }
	// for(auto& v : vertices) {
	// 	std::cout << id << " - " << *v << " " << v->colorId << std::endl;
	// 	id++;
	// }


	// read translations
	pElement = pRoot->FirstChildElement("Translations");
	XMLElement *pTranslation = pElement->FirstChildElement("Translation");
	while (pTranslation != NULL)
	{
		Translation *translation = new Translation();

		pTranslation->QueryIntAttribute("id", &translation->translationId);

		str = pTranslation->Attribute("value");
		sscanf(str, "%lf %lf %lf", &translation->tx, &translation->ty, &translation->tz);

		translations.push_back(translation);

		pTranslation = pTranslation->NextSiblingElement("Translation");
	}


	// read scalings
	pElement = pRoot->FirstChildElement("Scalings");
	XMLElement *pScaling = pElement->FirstChildElement("Scaling");
	while (pScaling != NULL)
	{
		Scaling *scaling = new Scaling();

		pScaling->QueryIntAttribute("id", &scaling->scalingId);
		str = pScaling->Attribute("value");
		sscanf(str, "%lf %lf %lf", &scaling->sx, &scaling->sy, &scaling->sz);

		scalings.push_back(scaling);

		pScaling = pScaling->NextSiblingElement("Scaling");
	}


	
	// read rotations
	pElement = pRoot->FirstChildElement("Rotations");
	XMLElement *pRotation = pElement->FirstChildElement("Rotation");
	while (pRotation != NULL)
	{
		Rotation *rotation = new Rotation();

		pRotation->QueryIntAttribute("id", &rotation->rotationId);
		str = pRotation->Attribute("value");
		sscanf(str, "%lf %lf %lf %lf", &rotation->angle, &rotation->ux, &rotation->uy, &rotation->uz);

		rotations.push_back(rotation);

		pRotation = pRotation->NextSiblingElement("Rotation");
	}

	// read meshes
	pElement = pRoot->FirstChildElement("Meshes");

	XMLElement *pMesh = pElement->FirstChildElement("Mesh");
	while (pMesh != NULL)
	{
		Mesh *mesh = new Mesh();

		pMesh->QueryIntAttribute("id", &mesh->meshId);

		// read projection type
		str = pMesh->Attribute("type");

		if (strcmp(str, "wireframe") == 0) {
			mesh->type = 0;
		}
		else {
			mesh->type = 1;
		}

		// read mesh transformations
		XMLElement *pTransformations = pMesh->FirstChildElement("Transformations");
		XMLElement *pTransformation = pTransformations->FirstChildElement("Transformation");

		while (pTransformation != NULL)
		{
			char transformationType;
			int transformationId;

			str = pTransformation->GetText();
			sscanf(str, "%c %d", &transformationType, &transformationId);

			mesh->transformationTypes.push_back(transformationType);
			mesh->transformationIds.push_back(transformationId);

			pTransformation = pTransformation->NextSiblingElement("Transformation");
		}
		
		mesh->numberOfTransformations = mesh->transformationIds.size();

		// read mesh faces
		char *row;
		char *clone_str;
		int v1, v2, v3;
		XMLElement *pFaces = pMesh->FirstChildElement("Faces");
        str = pFaces->GetText();
		clone_str = strdup(str);

		row = strtok(clone_str, "\n");
		int id = 0;
		while (row != NULL)
		{
			sscanf(row, "%d %d %d", &v1, &v2, &v3);
			v1--;
			v2--;
			v3--;
			mesh->triangles.push_back(Triangle(v1, v2, v3));

			mesh->triangles[id++].normal = Vec3::normalized(
				Vec3::cross(*(vertices[v3]) - *(vertices[v2]),
							*(vertices[v1]) - *(vertices[v2]))
				);
			
			row = strtok(NULL, "\n");
		}
		mesh->numberOfTriangles = mesh->triangles.size();
		meshes.push_back(mesh);

		pMesh = pMesh->NextSiblingElement("Mesh");
	}

}

/*
	Initializes image with background color
*/
void Scene::initializeImage(Camera *camera)
{
	if (this->image.empty())
	{
		for (int i = 0; i < camera->horRes; i++)
		{
			vector<Color> rowOfColors;

			for (int j = 0; j < camera->verRes; j++)
			{
				rowOfColors.push_back(this->backgroundColor);
			}

			this->image.push_back(rowOfColors);
		}
	}
	else
	{
		for (int i = 0; i < camera->horRes; i++)
		{
			for (int j = 0; j < camera->verRes; j++)
			{
				this->image[i][j].r = this->backgroundColor.r;
				this->image[i][j].g = this->backgroundColor.g;
				this->image[i][j].b = this->backgroundColor.b;
			}
		}
	}
}

/*
	If given value is less than 0, converts value to 0.
	If given value is more than 255, converts value to 255.
	Otherwise returns value itself.
*/
int Scene::makeBetweenZeroAnd255(double value)
{
	if (value >= 255.0)
		return 255;
	if (value <= 0.0)
		return 0;
	return (int)(value);
}

/*
	Writes contents of image (Color**) into a PPM file.
*/
void Scene::writeImageToPPMFile(Camera *camera)
{
	ofstream fout;

	fout.open(camera->outputFileName.c_str());
	fout << "P3" << endl;
	fout << "# " << camera->outputFileName << endl;
	fout << camera->horRes << " " << camera->verRes << endl;
	fout << "255" << endl;

	for (int j = camera->verRes - 1; j >= 0; j--)
	{
		for (int i = 0; i < camera->horRes; i++)
		{
			fout << makeBetweenZeroAnd255(this->image[i][j].r) << " "
				 << makeBetweenZeroAnd255(this->image[i][j].g) << " "
				 << makeBetweenZeroAnd255(this->image[i][j].b) << " ";
		}
		fout << endl;
	}
	fout.close();
}

/*
	Converts PPM image in given path to PNG file, by calling ImageMagick's 'convert' command.
	os_type == 1 		-> Ubuntu
	os_type == 2 		-> Windows
	os_type == other	-> No conversion
*/
void Scene::convertPPMToPNG(string ppmFileName, int osType)
{
	string command;

	// call command on Ubuntu
	if (osType == 1)
	{
		command = "convert " + ppmFileName + " " + ppmFileName + ".png";
		system(command.c_str());
	}
	// call command on Windows
	else if (osType == 2)
	{
		command = "magick convert " + ppmFileName + " " + ppmFileName + ".png";
		system(command.c_str());
	}
}

Matrix4 Scene::CreateTransformationMatrix(int transformationId, char type) {
	if(type == 's') { // SCALE
		auto& s = *scalings[transformationId];
		double pRes[4][4] = {
			{s.sx, 0,    0,    0},
			{0,    s.sy, 0,    0},
			{0,    0,    s.sz, 0},
			{0,    0,    0,    1}
		};
		Matrix4 res(pRes);
		return res;
	}
	else if(type == 't') { //  TRANSLATION
		auto& t = *translations[transformationId];
		double pRes[4][4] = {
			{1, 0, 0, t.tx},
			{0, 1, 0, t.ty},
			{0, 0, 1, t.tz},
			{0, 0, 0,    1}
		};
		Matrix4 res(pRes);
		return res;
	}
	else if(type == 'r') { // ROTATION
		auto& r = *rotations[transformationId];

		double angle = (r.angle / 180) * M_PI;
		double cosine = cos(angle), sine = sin(angle);

		Vec3   w(r.ux, r.uy, r.uz, 0);
		w.normalize();

		Vec3   t = w;
		double min_val = t.x;
		if( t.y <= min_val )
			min_val = t.y;
		if( t.z <= min_val )
			min_val = t.z;
		if( min_val == t.x )
			t.x = 1;
		else if( min_val == t.y )
			t.y = 1;
		else if( min_val == t.z )
			t.z = 1;

		Vec3 u = Vec3::normalized( Vec3::cross(t, w) );
		Vec3 v = Vec3::cross(w, u);

		double pRes1[4][4] = {
			{u.x, v.x, w.x, 0},
			{u.y, v.y, w.y, 0},
			{u.z, v.z, w.z, 0},
			{  0,   0,   0, 1}
		};
		Matrix4 m1(pRes1);

		double pRes2[4][4] = {
			{cosine,  -sine, 0, 0},
			{  sine, cosine, 0, 0},
			{     0,      0, 1, 0},
			{     0,      0, 0, 1}
		};
		Matrix4 m2(pRes2);
		
		double pRes3[4][4] = {
			{u.x, u.y, u.z, 0},
			{v.x, v.y, v.z, 0},
			{w.x, w.y, w.z, 0},
			{  0,   0,   0, 1}
		};
		Matrix4 m3(pRes3);

		Matrix4 res = getIdentityMatrix();
		res = multiplyMatrixWithMatrix(m3, res);
		res = multiplyMatrixWithMatrix(m2, res);
		res = multiplyMatrixWithMatrix(m1, res);
		
		return res;
	}
	else
		throw std::runtime_error("no such type of transform");
}

bool Scene::isCulledOff(Vec4& v1, Vec4& v2, Vec4& v3, Camera *camera) {
	Vec3 v1_(v1.x, v1.y, v1.z, v1.colorId); 
	Vec3 v2_(v2.x, v2.y, v2.z, v2.colorId); 
	Vec3 v3_(v3.x, v3.y, v3.z, v3.colorId); 

	Vec3 center = (v1_ + v2_ + v3_);
	center = multiplyVec3WithScalar(center, 1.0/3.0);
//	center = camera->pos - center;
	center.normalize();

	
	Vec3 normal = Vec3::cross(v2_ - v1_, v3_ - v1_);
	return (Vec3::dot(center, normal) >= 0.0);
}

// uint8_t getOutcode(Vec4& v1, Camera* camera) {
// 	uint8_t outcode1 = 0b00000000;
// 	if( v1.z > camera->far    * v1.t) outcode1 |= 0b00100000;
// 	if( v1.z < camera->near   * v1.t) outcode1 |= 0b00010000;	
// 	if( v1.y < camera->bottom * v1.t) outcode1 |= 0b00001000;
// 	if( v1.y > camera->top    * v1.t) outcode1 |= 0b00000100;
// 	if( v1.x < camera->left   * v1.t) outcode1 |= 0b00000010;	
// 	if( v1.x > camera->right  * v1.t) outcode1 |= 0b00000001;
// 	return outcode1;
// }

// // clipping alg 1
// bool Scene::CohenSutherland_Clipping(Vec4& v1, Vec4& v2, Camera* camera){
// 	uint8_t outcode1 = getOutcode(v1, camera);
// 	uint8_t outcode2 = getOutcode(v2, camera);
// 	uint8_t zero     = 0b00000000;
	
// 	if( (outcode1 | outcode2 ) == zero ) // trivial accept
// 		return true;

// 	if( (outcode1 & outcode2 ) != zero ) // trivial reject
// 		return false;

// 	while( true ) {
// 		if( outcode1 == zero )
// 			break;

// 		if( outcode1 & 0b00100000 != zero ) 
// 	}

// 	while( true ) {
// 		if( outcode2 == zero )
// 			break;

		
// 	}

// 	return true;
// }


// // clipping alg 2
// // clp_2() { clip }

// // SOME SORT OF CLIPPING FUNCTION (only for lines)
// bool Scene::Clipping(Vec4& v1, Vec4& v2, Camera* cam) { 
// 	return CohenSutherland_Clipping(v1, v2, cam);
// }




