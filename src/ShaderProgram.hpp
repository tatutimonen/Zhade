#pragma once

#include "Util.hpp"
#include "Shader.hpp"

#include <GL/glew.h>

#include <algorithm>
#include <array>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>

//------------------------------------------------------------------------

class ShaderProgram {
public:
    ShaderProgram(std::shared_ptr<Shader> vertexShader,
                  std::shared_ptr<Shader> fragmentShader,
                  std::shared_ptr<Shader> geometryShader = nullptr);
    ~ShaderProgram();

    template<typename T>
    void setUniform(const std::string& name, const GLvoid* data);

    inline GLuint getHandle() const noexcept { return m_handle; }
    GLint getAttribLocation(const std::string& name);
    GLint getUniformLocation(const std::string& name);

    void use() const noexcept;
    void unuse() const noexcept;

    void attachShader(const std::shared_ptr<Shader> shader);
    void detachShader(const std::shared_ptr<Shader> shader);
    void detachShaders();
    void link() const;

private:
    GLuint m_handle;
    std::array<std::shared_ptr<Shader>, Shader::N> m_shaders = { nullptr };
    std::unordered_map<std::string, GLint> m_attribLocationCache;
    std::unordered_map<std::string, GLint> m_uniformLocationCache;
};

//------------------------------------------------------------------------
