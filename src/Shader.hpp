#pragma once

#include "common.hpp"

#include <GL/glew.h>

#include <cassert>
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>

//------------------------------------------------------------------------
// Only support geometry stage as extra at this point.

template<const GLenum ShaderType>
concept IsValidGlShaderType = (
    ShaderType == GL_VERTEX_SHADER
        || ShaderType == GL_GEOMETRY_SHADER
        || ShaderType == GL_FRAGMENT_SHADER
);

//------------------------------------------------------------------------

template<const GLenum ShaderType>
requires IsValidGlShaderType<ShaderType>
class Shader
{
public:
    enum class DataType
    {
        Float, Vec2f, Vec3f, Vec4f, Mat3f, Mat4f,
        Int,   Vec2i, Vec3i, Vec4i,
        Bool,
        Uint_2_10_10_10_Rev
    };

    template<DataType T>
    static constexpr size_t getDataTypeSizeBytes()
    {
        switch constexpr (T)
        {
        case DataType::Float:
        case DataType::Int:
        case DataType::Uint_2_10_10_10_Rev:
                                            return 4;
        case DataType::Vec2f:
        case DataType::Vec2i:
                                            return 4 * 2;
        case DataType::Vec3f:
        case DataType::Vec3i:
                                            return 4 * 3;
        case DataType::Vec4f:
        case DataType::Vec4i:
                                            return 4 * 4;
        case DataType::Mat3f:
                                            return 4 * 3 * 3;
        case DataType::Mat4f:
                                            return 4 * 4 * 4;
        case DataType::Bool:
                                            return 1;
        default:
                                            return 0;
        }
    }

    template<DataType T>
    static constexpr uint32_t getDataTypeNumElements()
    {
        switch constexpr (T)
        {
        case DataType::Float:
        case DataType::Int:
                                            return 1;
        case DataType::Vec2f:
        case DataType::Vec2i:
                                            return 2;
        case DataType::Vec3f:
        case DataType::Vec3i:
                                            return 3;
        case DataType::Vec4f:
        case DataType::Vec4i:
        case DataType::Uint_2_10_10_10_Rev:
                                            return 4;
        case DataType::Mat3f:
                                            return 3 * 3;
        case DataType::Mat4f:
                                            return 4 * 4;
        case DataType::Bool:
                                            return 1;
        default:
                                            return 0;
        }
    }

    template<DataType T>
    static constexpr GLenum getDataTypeAsGlEnum()
	{
		switch constexpr (T)
		{
		case DataType::Float:
        case DataType::Vec2f:
        case DataType::Vec3f:
        case DataType::Vec4f:
        case DataType::Mat3f:
        case DataType::Mat4f:
                                            return GL_FLOAT;
        case DataType::Int:
        case DataType::Vec2i:
        case DataType::Vec3i:
        case DataType::Vec4i:
                                            return GL_INT;
        case DataType::Bool:
                                            return GL_BOOL;
        case DataType::Uint_2_10_10_10_Rev:
                                            return GL_UNSIGNED_INT_2_10_10_10_REV;
        default:
                                            return 0;
		}
	}

    Shader(std::string_view filename)
    {
        parseShaderFile(filename);
        m_handle = glCreateShader(ShaderType);
        // glShaderSource needs an lvalue.
        const char* shaderSourcePtr = m_shaderSource.c_str();
        glShaderSource(m_handle, 1, &shaderSourcePtr, nullptr);
        compile();
    }

    ~Shader()
    {
        glDeleteShader(m_handle)
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
        
        shaderFile.open(filename);
        std::ostringstream osstream;
        osstream << shaderFile.rdbuf();
        m_shaderSource = osstream.str();
        shaderFile.close();
    }

    void compile() const
    {
        glCompileShader(m_handle);
        Glint status;
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
