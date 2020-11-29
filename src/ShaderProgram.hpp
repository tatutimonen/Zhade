#pragma once

#include "Shader.hpp"
class Shader;

#include <GL/glew.h>

#include <cstdint>


class ShaderProgram {
    static constexpr uint32_t pipeline_length = 4;
public:
    ShaderProgram(Shader* vertex_shader, Shader* fragment_shader, Shader* geometry_shader = nullptr);
    ~ShaderProgram();

    uint32_t get_handle() const;
    void link() const;
    void use() const;

private:
    uint32_t m_handle;
    Shader* m_shaders[pipeline_length] = { nullptr };

    void delete_shaders();
};