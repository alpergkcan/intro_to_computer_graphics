//
// Created by alper on 05-Feb-22.
//

#include "window.h"
#include "Callbacks.h"

GLFWwindow*  Window::window  = nullptr;
GLFWmonitor* Window::monitor = nullptr;
int * Window::monitor_height = new int(0);
int * Window::monitor_width  = new int(0);
int Window::width  = 1000;
int Window::height = 1000;
int Window::pos_x  = 20;
int Window::pos_y  = 30;

bool Window::Init() {    if ( !glfwInit() ) {
        glfwTerminate();
        std::cout << "error::GLFW\n";
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);

    Window::monitor = glfwGetPrimaryMonitor();

    const GLFWvidmode* vmode = glfwGetVideoMode(monitor);
    *Window::monitor_width  = vmode->width;
    *Window::monitor_height = vmode->height;

    Window::window = glfwCreateWindow(Window::width, Window::height, "hw3", NULL, NULL);
	glfwSetWindowMonitor(window, NULL, Window::pos_x, Window::pos_y, Window::width, Window::height, 60);

    if( Window::window == NULL ) {
        std::cout << "error::GLFWwindow\n";
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, Callbacks::Resize);
    glfwSetKeyCallback(window, Callbacks::Key);
    glfwSetWindowPosCallback(window, Callbacks::WindowPos);

    if ( !gladLoadGLLoader( (GLADloadproc) glfwGetProcAddress ) ) {
        std::cout << "error::GLAD\n";
        glfwTerminate();
        return false;
    }

    // glfwSetInputMode(window, GLFW_STICKY_KEYS, true);
    glfwSetWindowPos(Window::window, 20, 40);
    return true;
}
