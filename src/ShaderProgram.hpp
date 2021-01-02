#pragma once

#include "Shader.hpp"
class Shader;

#include <GL/glew.h>

#include <utility>
#include <iostream>
#include <string>
#include <unordered_map>


class ShaderProgram {
    static constexpr unsigned int s_pipeline_length = 4u;
public:
    ShaderProgram(std::shared_ptr<Shader> vertex_shader,
                  std::shared_ptr<Shader> fragment_shader,
                  std::shared_ptr<Shader> geometry_shader = nullptr);
    ~ShaderProgram();

    GLuint get_handle() const { return m_handle; }
    GLint get_attrib_location(const std::string& name);
    GLint get_uniform_location(const std::string& name);
    void link() const;
    void use() const;
    void delete_shaders();

private:
    GLuint m_handle;
    std::shared_ptr<Shader> m_shaders[s_pipeline_length] = { nullptr };
    std::unordered_map<std::string, GLint> m_attrib_location_cache;
    std::unordered_map<std::string, GLint> m_uniform_location_cache;
};
