#include "input.h"

#include "../libraries/include/GLFW/glfw3.h"
#include "../libraries/include/glm/glm.hpp"
#include "../libraries/include/glm/ext.hpp"

#include "variables.h"

Input* Input::I;

Input::Input() {
	Input::I = this;

	keys = new std::vector<bool>(350);
	for(auto key:*keys) {
		key = false;
	}
}

bool Input::getKey(unsigned int glfw_key_code) {
	if( glfw_key_code >= 0  && glfw_key_code <= 350)
		return (*keys)[glfw_key_code];
	return false;
}


void Input::KeyboardInput() {
	Variables* I  = Variables::I;
	Input*     In = Input::I;

	
	// Heightmap
    if( In->getKey(GLFW_KEY_R) )
		I->heightFactor+=10;
    if( In->getKey(GLFW_KEY_F) )
		I->heightFactor-=10;

	// Turn
	if( In->getKey(GLFW_KEY_D) ) {
		glm::vec4 tmp = glm::rotate(I->Identity, I->turnAngle, I->up) * glm::vec4(I->gaze, 1.0f);
		I->gaze.x = tmp.x;
		I->gaze.y = tmp.y;
		I->gaze.z = tmp.z;
		I->right = glm::normalize(glm::cross(I->up, I->gaze));
	}
	if( In->getKey(GLFW_KEY_A) ) {
		glm::vec4 tmp = glm::rotate(I->Identity, -I->turnAngle, I->up) * glm::vec4(I->gaze, 1.0f);
		I->gaze.x = tmp.x;
		I->gaze.y = tmp.y;
		I->gaze.z = tmp.z;
		I->right = glm::normalize(glm::cross(I->up, I->gaze));
	}

	if( In->getKey(GLFW_KEY_W) ) {
		glm::vec4 tmp = glm::rotate(I->Identity, I->turnAngle, I->right) * glm::vec4(I->gaze, 1.0f);
		I->gaze.x = tmp.x;
		I->gaze.y = tmp.y;
		I->gaze.z = tmp.z;
		I->up = glm::normalize(glm::cross(I->gaze, I->right));
	}

	if( In->getKey(GLFW_KEY_S) ) {
		glm::vec4 tmp = glm::rotate(I->Identity, -I->turnAngle, I->right) * glm::vec4(I->gaze, 1.0f);
		I->gaze.x = tmp.x;
		I->gaze.y = tmp.y;
		I->gaze.z = tmp.z;
		I->up = glm::normalize(glm::cross(I->gaze, I->right));
	}

	// Plane
	if( In->getKey(GLFW_KEY_Y) )
		I->planeSpeed += 0.01f;
	if( In->getKey(GLFW_KEY_H) )
		I->planeSpeed -= 0.01f;
	if( In->getKey(GLFW_KEY_X) )
		I->planeSpeed = 0.0f;

	if( In->getKey(GLFW_KEY_I) ) {
		I->planeSpeed = 0.0f;
		I->eye = glm::vec3(0, 4000, 4000);
		I->up  = glm::vec3(0, 0, 1);
		I->gaze  = glm::normalize(glm::vec3(0, -1, -1));
		I->right = glm::normalize(glm::cross(I->up, I->gaze));
		I->up    = glm::normalize(glm::cross(I->gaze, I->right));
	}
	
}
