#pragma once

#include "Shader.hpp"

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <cstdint>
#include <iostream>
#include <optional>
#include <string>
#include <map>

//------------------------------------------------------------------------

class ShaderProgram {
public:
    ShaderProgram(const Shader& vertexShader, const Shader& fragmentShader,
        const std::optional<Shader>& geometryShader = std::nullopt);
    ~ShaderProgram();

    GLint getHandle() const noexcept     { return m_handle; }
    GLint getLinkStatus() const noexcept { return m_linkStatus; }
    GLint getUniformLocation(const std::string& name) const noexcept;

    void use() const noexcept   { glUseProgram(m_handle); }
    void unuse() const noexcept { glUseProgram(0); }

    template<typename T>
    void setUniform(const std::string& name, const void* data) const noexcept;

private:    
    void link() const noexcept;
    void detachShaders() const noexcept;

    GLuint m_handle;
    mutable GLint m_linkStatus;
    mutable std::map<std::string, GLint> m_attribLocationCache;
    mutable std::map<std::string, GLint> m_uniformLocationCache;
};

//------------------------------------------------------------------------
