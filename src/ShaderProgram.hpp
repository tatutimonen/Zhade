#pragma once

#include "Shader.hpp"
class Shader;

#include <GL/glew.h>

#include <iostream>
#include <string>
#include <unordered_map>


class ShaderProgram {
    static constexpr unsigned pipeline_length = 4u;
public:
    ShaderProgram(Shader* vertex_shader, Shader* fragment_shader, Shader* geometry_shader = nullptr);
    ~ShaderProgram();

    GLuint get_handle() const { return m_handle; }
    GLint get_uniform_location(const std::string& name);
    void link() const;
    void use() const;
    void delete_shaders();

private:
    GLuint m_handle;
    Shader* m_shaders[pipeline_length] = { nullptr };
    std::unordered_map<std::string, GLint> m_uniform_location_cache;
};
