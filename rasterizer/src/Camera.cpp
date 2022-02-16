#include "Camera.h"
#include "Helpers.h"
#include <string>
#include <iostream>
#include <iomanip>


using namespace std;

Camera::Camera() {}

Camera::Camera(int cameraId,
               int projectionType,
               Vec3 pos, Vec3 gaze,
               Vec3 u, Vec3 v, Vec3 w,
               double left, double right, double bottom, double top,
               double near, double far,
               int horRes, int verRes,
               string outputFileName)
{

    this->cameraId = cameraId;
    this->projectionType = projectionType;
    this->pos = pos;
    this->gaze = gaze;
    this->u = u;
    this->v = v;
    this->w = w;
    this->left = left;
    this->right = right;
    this->bottom = bottom;
    this->top = top;
    this->near = near;
    this->far = far;
    this->horRes = horRes;
    this->verRes = verRes;
    this->outputFileName = outputFileName;
}

Camera::Camera(const Camera &other)
{
    this->cameraId = other.cameraId;
    this->projectionType = other.projectionType;
    this->pos = other.pos;
    this->gaze = other.gaze;
    this->u = other.u;
    this->v = other.v;
    this->w = other.w;
    this->left = other.left;
    this->right = other.right;
    this->bottom = other.bottom;
    this->top = other.top;
    this->near = other.near;
    this->far = other.far;
    this->horRes = other.horRes;
    this->verRes = other.verRes;
    this->outputFileName = other.outputFileName;
}

ostream &operator<<(ostream &os, const Camera &c)
{
    const char *camType = c.projectionType ? "perspective" : "orthographic";

    os << fixed << setprecision(6) << "Camera " << c.cameraId << " (" << camType << ") => pos: " << c.pos << " gaze: " << c.gaze << endl
       << "\tu: " << c.u << " v: " << c.v << " w: " << c.w << endl
       << fixed << setprecision(3) << "\tleft: " << c.left << " right: " << c.right << " bottom: " << c.bottom << " top: " << c.top << endl
       << "\tnear: " << c.near << " far: " << c.far << " resolutions: " << c.horRes << "x" << c.verRes << " fileName: " << c.outputFileName;

    return os;
}


Matrix4 Camera::GetProjection() {
	double r_l = 1/(right - left);
	double t_b = 1/(top - bottom);
	double f_n = 1/(far -   near);
	double a_n  = abs(near);
	double a_f  = abs(far);	
	double a_nf = 1/( a_n - a_f);	
	if( projectionType ) { // 1 = perspective
		double pRes[4][4] =
			{
				{2 * a_n * r_l,  0,              (right + left  ) *  r_l,  0},
				{0,              2 * a_n * t_b,  (top   + bottom) *  t_b,  0},
				{0,              0,              (a_f   + a_n   ) * a_nf,  2 * a_f * a_n * a_nf},
				{0,              0,              -1,                       0}
			};
		Matrix4 res(pRes);
		return res;
	}
	else { // 0 = orthographic
		double pRes[4][4] =
			{
				{2 * r_l, 0,     0,      ((right + left  ) * r_l) * -1},
				{0,       2*t_b, 0,      ((top   + bottom) * t_b) * -1},
				{0,       0,     -2*f_n, ((far   + near  ) * f_n) *  1},
				{0,       0,     0,      1}
			};
		Matrix4 res(pRes);
		return res;
	}
}

Matrix4 Camera::GetView() {

	double pRes1[4][4] =
		{
			{u.x, u.y, u.z,	 0},
			{v.x, v.y, v.z,	 0},
			{w.x, w.y, w.z,	 0},
			{  0,   0,   0,  1}
		};
	Matrix4 m1(pRes1);

	double pRes2[4][4] =
		{
			{1, 0, 0, -pos.x},
			{0, 1, 0, -pos.y},
			{0, 0, 1, -pos.z},
			{0, 0, 0,      1}
			
		};
	Matrix4 m2(pRes2);

	Matrix4 res = getIdentityMatrix();
	res = multiplyMatrixWithMatrix(m2, res);
	res = multiplyMatrixWithMatrix(m1, res);	
	return res;
}

Matrix4 Camera::GetViewport() {
	double pRes[4][4] =
		{
			{horRes * 0.5,            0,  0, (horRes-1) * 0.5},
			{           0, verRes * 0.5,  0, (verRes-1) * 0.5},
			{           0,            0,  1,                0},
			{           0,            0,  0,                1}
		};
	Matrix4 res(pRes);
	return res;
}

