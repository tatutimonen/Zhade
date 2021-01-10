#pragma once

#include "gl_common.hpp"

#include <GL/glew.h>

#include <fstream>
#include <functional>
#include <iomanip>
#include <map>
#include <sstream>
#include <string>
#include <utility>


enum struct ShaderType {
    VERTEX_SHADER,
    GEOMETRY_SHADER,
    FRAGMENT_SHADER,
    COUNT
};

class Shader {
public:
    Shader(GLint gl_shader_type, const std::string& filename);
    ~Shader();

    inline GLuint get_handle() const          { return m_handle; }
    inline ShaderType get_shader_type() const { return m_shader_type; }

private:
    void parse_shader_file(const std::string& filename);
    void compile() const;

    GLuint m_handle;
    ShaderType m_shader_type;
    std::string m_shader_source;
};
