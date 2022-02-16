//
// Created by alper on 05-Feb-22.
//

#include "shader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "../libraries/include/glad/glad.h"

Shader::Shader(const std::string &vertex_path, const std::string &fragment_path) {
    const char * vertexCode;
    const char * fragmentCode;
    std::ifstream vFile;
    std::ifstream fFile;
    vFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    // read
    vFile.open( vertex_path);
    fFile.open(fragment_path);

    std::stringstream s_stream1, s_stream2;
    std::string s1, s2;
    s_stream1 << vFile.rdbuf();
    s1 = s_stream1.str();
    vertexCode = s1.c_str();

    s_stream2 << fFile.rdbuf();
    s2 = s_stream2.str();
    fragmentCode = s2.c_str();

    vFile.close();
    fFile.close();

    // create
    uint32_t VSO, FSO; // VertexShaderObject, FragmentShaderObject
    VSO = glCreateShader(GL_VERTEX_SHADER);
    FSO = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(VSO, 1, &vertexCode, nullptr);
    glShaderSource(FSO, 1, &fragmentCode, nullptr);
    glCompileShader(VSO);
    checkCompileErrors(VSO, "VERTEX");
    glCompileShader(FSO);
    checkCompileErrors(FSO, "FRAGMENT");

    ID = glCreateProgram();
    glAttachShader(ID, VSO);
    glAttachShader(ID, FSO);
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");

    // delete
    glDeleteShader(VSO);
    glDeleteShader(FSO);
}


void Shader::checkCompileErrors(unsigned int shader, const std::string &type) {
    char infoLog[1024];
    int success = true;
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog
                      << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog
                      << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    HasCompileErrors = !success;
}

void Shader::Use() {
    glUseProgram(ID);
}
