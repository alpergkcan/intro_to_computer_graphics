#include <iostream>
#include "../libraries/include/glad/glad.h"
#include "GLFW/glfw3.h"
#include "window.h"
#include "shader.h"
#include "Sphere.h"

#include "../libraries/include/glm/glm.hpp"
#include "../libraries/include/glm/matrix.hpp"
#include "../libraries/include/glm/ext.hpp"
#include "variables.h"

#include "texture.h"
#include "input.h"

int main(int argc, char** argv) {
    if( !Window::Init() )
        exit(1);

    Variables variables; Variables* I  = Variables::I;
	Input     inputs;    Input*     In = Input::I;


    I->pers = glm::perspective(45.0f, 1.0f, 0.1f, 10000.0f);

	I->lightPos = glm::vec3(0, 4000, 0);
	I->eye = glm::vec3(0, 4000, 4000);
	I-> up = glm::vec3(0,    0,    1);
	I->axialTurnFactor = 0.002f;
	
	I->gaze  = glm::normalize(glm::vec3(0, -1, -1));
	I->right = glm::normalize(glm::cross(I->up, I->gaze));
	I->up    = glm::normalize(glm::cross(I->gaze, I->right));

    // moon  : ====> vp translate lightPos  postRotate preRotate
    // earth : ====> vp lightPos  preRotate

	Texture earthTexture(argv[2], I->earthS.ID, "TexColor", 0);
	Texture heightMap   (argv[1], I->earthS.ID, "TexGrey",  1);
	Texture moonTexture (argv[3],  I->moonS.ID, "MoonTexColor", 0);
	
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glPolygonMode(GL_FRONT, GL_FILL);
    glfwSwapInterval(1.0f);

    I->iteration = 0;
    glClearColor( 0, 0, 0, 1);
    while ( !glfwWindowShouldClose(Window::window) )
    {
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		I->eye = I->eye + I->gaze * I->planeSpeed;
		I->view = glm::lookAt(I->eye, I->eye + I->gaze, I->up);
		I->vp   = I->pers * I->view;

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, moonTexture.textureID);
		I->DrawMoon();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, earthTexture.textureID);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, heightMap.textureID);
		I->DrawEarth();

		In->KeyboardInput();
			
        glfwSwapBuffers(Window::window);
        glfwPollEvents();
        I->iteration++;
	}

	
    glfwTerminate();
    return 0;
}


