//
// Created by alper on 06-Feb-22.
//

#ifndef HW3_PROJ_VARIABLES_H
#define HW3_PROJ_VARIABLES_H
#include <glm/ext.hpp>
#include <glm/matrix.hpp>
#include <glm/glm.hpp>
#include "shader.h"
#include "Sphere.h"

class Variables{
public:
	static Variables* I;

public:
    glm::vec3 eye  = glm::vec3(0, 4000, 4000);
    glm::vec3 gaze = glm::vec3(0,   -1,   -1);
	glm::vec3 right;
    glm::vec3 up = glm::vec3(0, 0, 1);

    glm::vec3 z_axis = glm::vec3(0, 0, 1);
    glm::vec3 center = glm::vec3(0, 0, 0);

	glm::vec3 lightPos = glm::vec3(0, 4000, 0);

	glm::mat4 pers;
	glm::mat4 view;
	glm::mat4 vp;

	glm::mat4 Identity = glm::mat4(1);

	Shader moonS = Shader("rsrc/shader/moon.vert", "rsrc/shader/moon.frag");
	Shader earthS = Shader("rsrc/shader/earth.vert", "rsrc/shader/earth.frag");

	Sphere moon = Sphere(162);
	Sphere earth = Sphere(600);

	glm::mat4 mvp_earth, mvp_moon;

    float heightFactor = 80;
	float axialTurnFactor = 0.5f/250;
	float turnAngle = -0.05f;
	float planeSpeed = 0.0f;
	int iteration = 0;

	void DrawMoon();
	void DrawEarth();



public:
	Variables();
};

#endif //HW3_PROJ_VARIABLES_H
