#pragma once

#include "ShaderProgram.hpp"
class ShaderProgram;

#include <GL/glew.h>

#include <cstdint>
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
    Shader(const std::string& filename, int32_t gl_shader_type);
    ~Shader();

    uint32_t get_handle() const;
    void attach(const ShaderProgram* shader_program) const;

private:
    std::string m_shader_source;
    uint32_t m_handle;
};