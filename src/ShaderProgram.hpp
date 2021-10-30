#pragma once

#include "Shader.hpp"
#include "Util.hpp"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include <algorithm>
#include <array>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>

//------------------------------------------------------------------------

class ShaderProgram {
public:
    ShaderProgram(const std::shared_ptr<Shader>& vertexShader = std::make_shared<Shader>(GL_VERTEX_SHADER),
        const std::shared_ptr<Shader>& fragmentShader = std::make_shared<Shader>(GL_FRAGMENT_SHADER),
        const std::shared_ptr<Shader>& geometryShader = nullptr);
    ~ShaderProgram();

    template<typename T>
    void setUniform(const std::string& name, const GLvoid* data) noexcept;

    inline GLuint getHandle() const noexcept { return m_handle; }
    GLint getAttribLocation(const std::string& name) noexcept;
    GLint getUniformLocation(const std::string& name) noexcept;

    inline void use() const noexcept   { CHECK_GL_ERROR(glUseProgram(m_handle)); }
    inline void unuse() const noexcept { CHECK_GL_ERROR(glUseProgram(0)); };

    void attachShader(const std::shared_ptr<Shader>& shader);
    void detachShader(const std::shared_ptr<Shader>& shader);
    void detachShaders();
    void link() const;

    inline bool operator<(const ShaderProgram& other) const noexcept { return m_handle < other.getHandle(); }

private:    
    GLuint m_handle;
    std::array<std::shared_ptr<Shader>, Shader::N> m_shaders = { nullptr };
    std::unordered_map<std::string, GLint> m_attribLocationCache;
    std::unordered_map<std::string, GLint> m_uniformLocationCache;
};

//------------------------------------------------------------------------
