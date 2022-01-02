#pragma once

#include "common.hpp"

#include <GL/glew.h>

#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

//------------------------------------------------------------------------

class Shader {
public:
    virtual ~Shader() = default;

    Shader(const Shader&) = delete;
    Shader(Shader&&) = delete;
    Shader& operator=(const Shader&) = delete;
    Shader& operator=(Shader&&) = delete;

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
    
    ShaderImpl(const ShaderImpl&) = delete;
    ShaderImpl(ShaderImpl&&) = delete;
    ShaderImpl& operator=(const ShaderImpl&) = delete;
    ShaderImpl& operator=(ShaderImpl&&) = delete;

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
    
    ShaderNull(const ShaderNull&) = delete;
    ShaderNull(ShaderNull&&) = delete;
    ShaderNull& operator=(const ShaderNull&) = delete;
    ShaderNull& operator=(ShaderNull&&) = delete;

    virtual inline void attach(uint32_t program) const noexcept override {}
    virtual inline void detach(uint32_t program) const noexcept override {}
};

//------------------------------------------------------------------------
