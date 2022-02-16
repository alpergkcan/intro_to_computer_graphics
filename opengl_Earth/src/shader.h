//
// Created by alper on 05-Feb-22.
//

#ifndef HW3_PROJ_SHADER_H
#define HW3_PROJ_SHADER_H
#include <string>

class Shader {
public:
    Shader(const std::string &vertex_shader, const std::string& fragment_shader);
    void Use();
    void checkCompileErrors(unsigned int shader, const std::string &type);

public:
    unsigned int ID;

private:
    bool HasCompileErrors;
};

#endif //HW3_PROJ_SHADER_H
