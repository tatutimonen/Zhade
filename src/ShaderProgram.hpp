#pragma once

#include "Shader.hpp"
class Shader;

#include <GL/glew.h>

#include <iostream>


class ShaderProgram {
    static constexpr unsigned pipeline_length = 4U;
public:
    ShaderProgram(Shader* vertex_shader, Shader* fragment_shader, Shader* geometry_shader = nullptr);
    ~ShaderProgram();

    GLuint get_handle() const;
    void link() const;
    void use() const;

private:
    GLuint m_handle;
    Shader* m_shaders[pipeline_length] = { nullptr };

    void delete_shaders();
};