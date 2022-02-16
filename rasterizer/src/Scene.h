#ifndef _SCENE_H_
#define _SCENE_H_

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

#include "Camera.h"
#include "Color.h"
#include "Mesh.h"
#include "Rotation.h"
#include "Scaling.h"
#include "Translation.h"
#include "Triangle.h"
#include "Vec3.h"
#include "Vec4.h"
#include "Line.h"
#include "Matrix4.h"

using namespace std;

struct Scene
{
	Color backgroundColor;
	bool cullingEnabled;

	vector< vector<Color> > image;

	vector< Camera* > cameras;
	vector< Vec3* > vertices;
	vector< Color* > colorsOfVertices;

	vector< Scaling* > scalings;
	vector< Rotation* > rotations;
	vector< Translation* > translations;
	vector< Mesh* > meshes;

	Scene(const char *xmlPath);

	void initializeImage(Camera* camera);
	void forwardRenderingPipeline(Camera* camera);
   	int  makeBetweenZeroAnd255(double value);
	void writeImageToPPMFile(Camera* camera);
	void convertPPMToPNG(string ppmFileName, int osType);
	
	bool isCulledOff(Vec4& v1, Vec4& v2, Vec4& v3o, Camera* camera);
	Matrix4 CreateTransformationMatrix(int transformationId, char type);

	// bool CohenSutherland_Clipping(Vec4& v1, Vec4& v2, Camera* camera);
	// bool Clipping(Vec4& v1, Vec4& v2, Camera* cam);
	void TriangleRasterizer(Vec4& v1, Vec4& v2, Vec4& v3);
	void LineRasterizer( Vec4& p1, Vec4& p2);
	void Rasterize(Vec4& v1, Vec4& v2, Vec4& v3, bool isSolid);
	void paint(double x_loc, double y_loc, Color color_rgb);

};

#endif
