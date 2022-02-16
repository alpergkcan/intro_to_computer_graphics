//
// Created by alper on 05-Feb-22.
//

#include "Callbacks.h"
#include "window.h"
#include "variables.h"
#include "input.h"

void Callbacks::Resize(GLFWwindow* window, int w, int h) {
    glViewport(0, 0, w, h);
	Variables* I = Variables::I;
	I->pers = glm::perspective(45.0f, (float)w/h, 0.1f, 10000.0f);
	I->vp = I->pers * I->view;
    if(glfwGetWindowMonitor(window))
        return;

    Window::width = w;
    Window::height = h;
}

void Callbacks::Key(GLFWwindow* window, int key, int scancode, int action, int modes) {
	Input*     In = Input::I;
	if( key >= 0 )
		(*In->keys)[key] = (action == GLFW_PRESS || action == GLFW_REPEAT ) ? true: false;
	else
		return;
	
	if( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

	if( key == GLFW_KEY_P && action == GLFW_PRESS) {
        GLFWmonitor* m = glfwGetWindowMonitor(window);
        if ( m == NULL) {
            glfwSetWindowMonitor(window, Window::monitor, 0, 0, *Window::monitor_width, *Window::monitor_height, 60);
            glViewport(0, 0, *Window::monitor_width, *Window::monitor_height);
        }
        else {
            
            glfwSetWindowMonitor(window, NULL, Window::pos_x, Window::pos_y, Window::width, Window::height, 60);
            glViewport(0, 0, Window::width, Window::height );
        }
    }
}

void Callbacks::WindowPos(GLFWwindow *window, int x_pos, int y_pos) {
    if(glfwGetWindowMonitor(window))
        return;
    Window::pos_x = x_pos;
    Window::pos_y = y_pos;
}

