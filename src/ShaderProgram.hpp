#pragma once

#include "Shader.hpp"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include <cstdint>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

//------------------------------------------------------------------------

class ShaderProgram {
public:
    ShaderProgram(const Shader& vertexShader, const Shader& fragmentShader,
        const Shader& geometryShader = ShaderNull());
    ~ShaderProgram();

    inline uint32_t getHandle() const noexcept { return m_handle; }
    int32_t getAttribLocation(const std::string& name) noexcept;
    int32_t getUniformLocation(const std::string& name) noexcept;

    inline void use() const noexcept   { glUseProgram(m_handle); }
    inline void unuse() const noexcept { glUseProgram(0); }

    template<typename T>
    void setUniform(const std::string& name, const void* data) noexcept;

private:    
    void link() noexcept;
    void detachShaders() const noexcept;

    uint32_t m_handle;
    int32_t m_linkStatus;
    std::unordered_map<std::string, int32_t> m_attribLocationCache;
    std::unordered_map<std::string, int32_t> m_uniformLocationCache;
};

//------------------------------------------------------------------------
