//
// Created by alper on 05-Feb-22.
//

#ifndef HW3_PROJ_CALLBACKS_H
#define HW3_PROJ_CALLBACKS_H

#include "../libraries/include/glad/glad.h"
#include <GLFW/glfw3.h>

namespace Callbacks {
    void Resize(GLFWwindow *window, int w, int h);
    void Key(GLFWwindow *window, int key, int scancode, int action, int modes);
    void WindowPos(GLFWwindow* window, int x_pos, int y_pos);
}
#endif //HW3_PROJ_CALLBACKS_H
