#pragma once

#include "common.hpp"

#include <GL/glew.h>

#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

//------------------------------------------------------------------------

class Shader {
public:
    virtual ~Shader() = default;
    Shader(const Shader&) = default;
    Shader(Shader&&) = default;
    Shader& operator=(const Shader&) = default;
    Shader& operator=(Shader&&) = default;

    virtual void attach(uint32_t program) const noexcept = 0;
    virtual void detach(uint32_t program) const noexcept = 0;

protected:
    Shader() = default;
};

//------------------------------------------------------------------------

class ShaderImpl final : public Shader {
public:
    ShaderImpl(uint32_t glShaderType, const std::string& filename);
    virtual ~ShaderImpl();
    ShaderImpl(const ShaderImpl&) = default;
    ShaderImpl(ShaderImpl&&) = default;
    ShaderImpl& operator=(const ShaderImpl&) = default;
    ShaderImpl& operator=(ShaderImpl&&) = default;

    inline uint32_t getHandle() const noexcept { return m_handle; }

    virtual inline void attach(uint32_t program) const noexcept override { glAttachShader(program, m_handle); }
    virtual inline void detach(uint32_t program) const noexcept override { glDetachShader(program, m_handle); }
    
private:
    void parseShaderFile(const std::string& filename);
    void compile() const;

    uint32_t m_handle;
    std::string m_shaderSource;
};

//------------------------------------------------------------------------

class ShaderNull final : public Shader {
public:
    ShaderNull() = default;
    virtual ~ShaderNull() = default;
    ShaderNull(const ShaderNull&) = default;
    ShaderNull(ShaderNull&&) = default;
    ShaderNull& operator=(const ShaderNull&) = default;
    ShaderNull& operator=(ShaderNull&&) = default;

    virtual inline void attach(uint32_t program) const noexcept override {}
    virtual inline void detach(uint32_t program) const noexcept override {}
};

//------------------------------------------------------------------------
