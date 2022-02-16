//
// Created by alper on 05-Feb-22.
//

#ifndef HW3_PROJ_WINDOW_H
#define HW3_PROJ_WINDOW_H

#include <iostream>
#include "../libraries/include/glad/glad.h"
#include <GLFW/glfw3.h>

struct Window {
    static GLFWwindow*   window;
    static GLFWmonitor* monitor;

    static int*  monitor_width;
    static int* monitor_height;

    static int  width;
    static int height;

    static int pos_x;
    static int pos_y;

    static bool Init();
};

#endif //HW3_PROJ_WINDOW_H
