//
// Created by alper on 06-Feb-22.
//
#define _USE_MATH_DEFINES

#include "Sphere.h"
#include "../libraries/include/glad/glad.h"
#include <cmath>

Sphere::~Sphere(){
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteBuffers(1, &VAO);
}

Sphere::Sphere(float radius) {
    std::vector<float> vertices;
    float hs = 2 * M_PI / 250;
    float vs = M_PI / 125;
    float pi_2 = M_PI * 2;

    // add vertices
	int h_step = 0;
    for(float alpha = 0; alpha < pi_2; alpha += hs) {
		int v_step = 0;
        for(float beta = 0; beta < M_PI; beta += vs) {
            vertices.push_back( radius * sin(beta) * cos(alpha) ); // x
            vertices.push_back( radius * sin(beta) * sin(alpha) ); // y
            vertices.push_back( radius * cos(beta) );              // z

			vertices.push_back((float)h_step/250); // tex_x
			vertices.push_back((float)v_step/125); // tex_y
			
			v_step++;
        }
		h_step++;
    }

    std::vector<unsigned int> indices;
    int pos, right, top;
    // add indices
    for(int x = 0; x < 250; x++) { // dikey
        for(int y = 0; y < 126; y++) { // yatay
            pos = 126 * x + y;
            top = pos - 1;
            right = pos + 126;

            indices.push_back( pos   );  // ben
            indices.push_back( right );  // sag
            indices.push_back( top   );  // ust

            indices.push_back( right   );  // sag
            indices.push_back( right-1 );  // sag ust
            indices.push_back( top     );  // ust
        }
    }

    count = indices.size();

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

void Sphere::Draw() {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);
}
