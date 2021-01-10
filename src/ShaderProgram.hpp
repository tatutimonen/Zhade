#pragma once

#include "gl_common.hpp"
#include "Shader.hpp"

#include <GL/glew.h>

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>


class ShaderProgram {
public:
    ShaderProgram(std::shared_ptr<Shader> vertex_shader,
                  std::shared_ptr<Shader> fragment_shader,
                  std::shared_ptr<Shader> geometry_shader = nullptr);
    ~ShaderProgram();

    template<typename T>
    void set_uniform(const std::string& name, const float* data);

    inline GLuint get_handle() const { return m_handle; }
    GLint get_attrib_location(const std::string& name);
    GLint get_uniform_location(const std::string& name);

    void link() const;
    void use() const;
    void attach_shader(std::shared_ptr<Shader> shader);
    void detach_shader(std::shared_ptr<Shader> shader);
    void detach_shaders();

private:
    GLuint m_handle;
    std::shared_ptr<Shader> m_shaders[static_cast<unsigned int>(ShaderType::COUNT)] = { nullptr };
    std::unordered_map<std::string, GLint> m_attrib_location_cache;
    std::unordered_map<std::string, GLint> m_uniform_location_cache;
};
