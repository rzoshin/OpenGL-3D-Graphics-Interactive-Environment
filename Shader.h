#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <iostream>

class Shader {
public:
    unsigned int ID;
    Shader(const std::string& vertexCode, const std::string& fragmentCode);
    void use() const;
    void setMat4(const std::string& name, const glm::mat4& mat) const;
    void setVec3(const std::string& name, const glm::vec3& vec) const;
    void setInt(const std::string& name, int value) const;
    void setBool(const std::string& name, bool value) const;
    void setFloat(const std::string& name, float value) const;

private:
    void checkCompileErrors(unsigned int shader, const std::string& type);
};

#endif