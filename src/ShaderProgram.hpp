#pragma once

#include "Shader.hpp"
class Shader;

#include <GL/glew.h>

#include <iostream>


class ShaderProgram {
    static constexpr unsigned int pipeline_length = 4U;
public:
    ShaderProgram(Shader* vertex_shader, Shader* fragment_shader, Shader* geometry_shader = nullptr);
    ~ShaderProgram();

    GLuint get_handle() const;
    GLint get_uniform_location(const char* name) const;
    void link() const;
    void use() const;
    void delete_shaders();

private:
    GLuint m_handle;
    Shader* m_shaders[pipeline_length] = { nullptr };
};
