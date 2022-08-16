#pragma once

#include "common.hpp"

#include <GL/glew.h>

#include <fstream>
#include <sstream>
#include <string>
#include <string_view>

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

enum class ShaderDataType
{
    Float, Vec2f, Vec3f, Vec4f, Mat3f, Mat4f,
    Int,   Vec2i, Vec3i, Vec4i,
    Bool,
    Uint_2_10_10_10_Rev
};

template<ShaderDataType T>
static constexpr size_t getShaderDataTypeSizeBytes()
{
    switch constexpr (T)
    {
    case ShaderDataType::Float:
    case ShaderDataType::Int:
    case ShaderDataType::Uint_2_10_10_10_Rev:
                                              return 4;
    case ShaderDataType::Vec2f:
    case ShaderDataType::Vec2i:
                                              return 4 * 2;
    case ShaderDataType::Vec3f:
    case ShaderDataType::Vec3i:
                                              return 4 * 3;
    case ShaderDataType::Vec4f:
    case ShaderDataType::Vec4i:
                                              return 4 * 4;
    case ShaderDataType::Mat3f:
                                              return 4 * 3 * 3;
    case ShaderDataType::Mat4f:
                                              return 4 * 4 * 4;
    case ShaderDataType::Bool:
                                              return 1;
    default:
                                              return 0;
    }
}

template<ShaderDataType T>
static constexpr size_t getShaderShaderDataTypeSize()
{
    switch constexpr (T)
    {
    case ShaderDataType::Float:
    case ShaderDataType::Int:
                                              return 1;
    case ShaderDataType::Vec2f:
    case ShaderDataType::Vec2i:
                                              return 2;
    case ShaderDataType::Vec3f:
    case ShaderDataType::Vec3i:
                                              return 3;
    case ShaderDataType::Vec4f:
    case ShaderDataType::Vec4i:
    case ShaderDataType::Uint_2_10_10_10_Rev:
                                              return 4;
    case ShaderDataType::Mat3f:
                                              return 3 * 3;
    case ShaderDataType::Mat4f:
                                              return 4 * 4;
    case ShaderDataType::Bool:
                                              return 1;
    default:
                                              return 0;
    }
}

template<ShaderDataType T>
static constexpr GLenum getShaderShaderDataTypeAsGlEnum()
{
    switch constexpr (T)
    {
    case ShaderDataType::Float:
    case ShaderDataType::Vec2f:
    case ShaderDataType::Vec3f:
    case ShaderDataType::Vec4f:
    case ShaderDataType::Mat3f:
    case ShaderDataType::Mat4f:
                                              return GL_FLOAT;
    case ShaderDataType::Int:
    case ShaderDataType::Vec2i:
    case ShaderDataType::Vec3i:
    case ShaderDataType::Vec4i:
                                              return GL_INT;
    case ShaderDataType::Bool:
                                              return GL_BOOL;
    case ShaderDataType::Uint_2_10_10_10_Rev:
                                              return GL_UNSIGNED_INT_2_10_10_10_REV;
    default:
                                              return 0;
    }
}

//------------------------------------------------------------------------
// Only support geometry stage as extra at this point.

template<GLenum ShaderType>
concept IsValidGlShaderType = (
    ShaderType == GL_VERTEX_SHADER
        || ShaderType == GL_GEOMETRY_SHADER
        || ShaderType == GL_FRAGMENT_SHADER
);

//------------------------------------------------------------------------

template<GLenum ShaderType>
requires IsValidGlShaderType<ShaderType>
class Shader
{
public:
    Shader(std::string_view filename)
    {
        parseShaderFile(filename);
        m_handle = glCreateShader(ShaderType);
        // glShaderSource needs an lvalue.
        const auto shaderSourcePtr = m_shaderSource.c_str();
        glShaderSource(m_handle, 1, &shaderSourcePtr, nullptr);
        compile();
    }

    ~Shader()
    {
        glDeleteShader(m_handle);
    }

    Shader(const Shader&) = default;
    Shader& operator=(const Shader&) = default;
    Shader(Shader&&) = default;
    Shader& operator=(Shader&&) = default;

    [[nodiscard]] GLuint getHandle() const noexcept { return m_handle; }

    void attach(const GLuint program) const noexcept { glAttachShader(program, m_handle); }
    void detach(const GLuint program) const noexcept { glDetachShader(program, m_handle); }
    
private:
    void parseShaderFile(std::string_view filename)
    {
        std::ifstream shaderFile;
        shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        
        shaderFile.open(filename.data());
        std::ostringstream osstream;
        osstream << shaderFile.rdbuf();
        m_shaderSource = osstream.str();
        shaderFile.close();
    }

    void compile() const
    {
        glCompileShader(m_handle);
        GLint status;
        glGetShaderiv(m_handle, GL_COMPILE_STATUS, &status);
        if (status == GL_FALSE)
        {
            GLint logLength;
            glGetShaderiv(m_handle, GL_INFO_LOG_LENGTH, &logLength);
            std::string infoLog;
            infoLog.resize(static_cast<std::string::size_type>(logLength - 1));
            glGetShaderInfoLog(m_handle, logLength, nullptr, infoLog.data());
            
            std::ostringstream errMsg;
            errMsg << "Error compiling shader with ID "
                    << m_handle
                    << ": "
                    << infoLog;
            throw std::runtime_error(errMsg.str());
        }
    }

    GLuint m_handle;
    std::string m_shaderSource;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
