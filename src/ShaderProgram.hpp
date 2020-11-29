#pragma once

#include "Shader.hpp"
class Shader;

#include <GL/glew.h>

#include <cstdint>

static constexpr uint32_t pipeline_length = 4;

enum ShaderType {
    VERTEX_SHADER,
    GEOMETRY_SHADER,
    TESSELLATION_SHADER,
    FRAGMENT_SHADER
};

class ShaderProgram {
public:
    ShaderProgram(const Shader& vertex_shader, const Shader& fragment_shader, const Shader* geometry_shader = nullptr);
    ~ShaderProgram();

    uint32_t get_handle() const;
    void link() const;
    void use() const;

private:
    uint32_t m_handle;
    uint32_t m_shader_handles[pipeline_length] = { 0 };
};