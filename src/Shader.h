#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <string>

class Shader
{
private:
    unsigned int m_id{};
    void checkCompileErrors(unsigned int shader, std::string_view type);

public:
    Shader(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath);
    void use();
    void setUniform(const std::string& name, bool value) const;
    void setUniform(const std::string& name, int value) const;
    void setUniform(const std::string& name, float value) const;
};

#endif
