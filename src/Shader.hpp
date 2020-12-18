#pragma once

#include "gl_common.hpp"
#include "ShaderProgram.hpp"
class ShaderProgram;

#include <GL/glew.h>

#include <string>
#include <fstream>
#include <sstream>


enum ShaderType {
    VERTEX_SHADER,
    GEOMETRY_SHADER,
    TESSELLATION_SHADER,
    FRAGMENT_SHADER
};

class Shader {
public:
    Shader(GLint gl_shader_type, const std::string& filename);
    ~Shader();

    GLuint get_handle() const;
    void attach(const ShaderProgram* shader_program);

private:
    std::string m_shader_source;
    GLuint m_handle;
};
