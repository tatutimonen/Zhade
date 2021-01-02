#pragma once

#include "gl_common.hpp"
#include "ShaderProgram.hpp"
class ShaderProgram;

#include <GL/glew.h>

#include <utility>
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

    GLuint get_handle() const { return m_handle; }
    void attach(const std::unique_ptr<ShaderProgram> shader_program);

private:
    void parse_shader_file(const std::string& filename);
    void compile() const;

    GLuint m_handle;
    std::string m_shader_source;
};
