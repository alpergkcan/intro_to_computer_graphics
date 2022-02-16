//
// Created by alper on 06-Feb-22.
//

#include "variables.h"
#include "../libraries/include/glad/glad.h"

Variables* Variables::I;

Variables::Variables() {
	Variables::I = this;
}

void Variables::DrawMoon() {
	moonS.Use();
	glm::mat4 model_moon;
	model_moon = glm::rotate(Identity, 0.02f * iteration, z_axis);
	model_moon = glm::translate(model_moon, glm::vec3(0, 2600, 0));
	model_moon = glm::rotate( model_moon, (float)iteration * axialTurnFactor, z_axis);
	glUniformMatrix4fv(glGetUniformLocation(moonS.ID,"vp"), 1, false, glm::value_ptr(vp));
	glUniformMatrix4fv(glGetUniformLocation(moonS.ID,"m"), 1, false, glm::value_ptr(model_moon));

	glUniform3fv(glGetUniformLocation(moonS.ID,  "lightPos"), 1, glm::value_ptr(lightPos));
	glUniform3fv(glGetUniformLocation(moonS.ID, "cameraPos"), 1, glm::value_ptr(eye));
	moon.Draw();
}

void Variables::DrawEarth() {
	earthS.Use();
	glm::mat4 model_earth;
	model_earth = glm::rotate(Identity, (float)iteration * axialTurnFactor, z_axis);
	glUniformMatrix4fv(glGetUniformLocation(earthS.ID,"vp"), 1, false, glm::value_ptr(vp));
	glUniformMatrix4fv(glGetUniformLocation(earthS.ID,"m"), 1, false, glm::value_ptr(model_earth));
    glUniform3fv(glGetUniformLocation(earthS.ID,    "lightPos"), 1, glm::value_ptr(lightPos));
	glUniform3fv(glGetUniformLocation(earthS.ID,   "cameraPos"), 1, glm::value_ptr(eye));
	glUniform1i(glGetUniformLocation(earthS.ID, "HeightFactor"), I->heightFactor);
	earth.Draw();
}
