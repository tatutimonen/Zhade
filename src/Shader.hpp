#pragma once

#include "Util.hpp"

extern "C" {
#include <GL/glew.h>
}
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>

// ([^m])_([a-z])
// $1\U$2

class Shader {
public:
    enum ShaderType {
        VERTEX_SHADER,
        GEOMETRY_SHADER,
        FRAGMENT_SHADER,
        SHADER_MAX
    };

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
