//
// Created by alper on 06-Feb-22.
//

#ifndef HW3_PROJ_SPHERE_H
#define HW3_PROJ_SPHERE_H
#include <iostream>
#include <vector>

class Sphere {
public:
    Sphere(float radius);
	~Sphere();
    void Draw();
public:
    unsigned int VBO, VAO, EBO;
	unsigned int shaderProgramID;
private:
    unsigned int count;
};


#endif //HW3_PROJ_SPHERE_H
