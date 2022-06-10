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

//------------------------------------------------------------------------

class Shader {
public:
    enum class DataType
    {
        Float, Vec2f, Vec3f, Vec4f, Mat3f, Mat4f,
        Int,   Vec2i, Vec3i, Vec4i,
        Bool,
        Uint_2_10_10_10_Rev
    };

    static std::size_t getDataTypeSizeBytes(const DataType type)
    {
        switch (type)
        {
        case DataType::Float:
        case DataType::Int:
        case DataType::Uint_2_10_10_10_Rev: return 4;
        case DataType::Vec2f:
        case DataType::Vec2i:               return 4 * 2;
        case DataType::Vec3f:
        case DataType::Vec3i:               return 4 * 3;
        case DataType::Vec4f:
        case DataType::Vec4i:               return 4 * 4;
        case DataType::Mat3f:               return 4 * 3 * 3;
        case DataType::Mat4f:               return 4 * 4 * 4;
        case DataType::Bool:                return 1;
        default:                            return 0;
        }
    }

    static std::uint32_t getDataTypeNumElements(const DataType type)
    {
        switch (type)
        {
        case DataType::Float:
        case DataType::Int:                 return 1;
        case DataType::Vec2f:
        case DataType::Vec2i:               return 2;
        case DataType::Vec3f:
        case DataType::Vec3i:               return 3;
        case DataType::Vec4f:
        case DataType::Vec4i:
        case DataType::Uint_2_10_10_10_Rev: return 4;
        case DataType::Mat3f:               return 3 * 3;
        case DataType::Mat4f:               return 4 * 4;
        case DataType::Bool:                return 1;
        default:                            return 0;
        }
    }

    static GLenum getDataTypeAsGlBaseType(const DataType type)
	{
		switch (type)
		{
		case DataType::Float:
        case DataType::Vec2f:
        case DataType::Vec3f:
        case DataType::Vec4f:
        case DataType::Mat3f:
        case DataType::Mat4f:               return GL_FLOAT;
        case DataType::Int:
        case DataType::Vec2i:
        case DataType::Vec3i:
        case DataType::Vec4i:               return GL_INT;
        case DataType::Bool:                return GL_BOOL;
        case DataType::Uint_2_10_10_10_Rev: return GL_UNSIGNED_INT_2_10_10_10_REV;
        default:                            return 0;
		}
	}

    Shader(const GLenum glShaderType, const std::string& filename);
    ~Shader();
    Shader(const Shader&) = default;
    Shader& operator=(const Shader&) = default;
    Shader(Shader&&) = default;
    Shader& operator=(Shader&&) = default;

    GLuint getHandle() const noexcept { return m_handle; }

    void attach(const GLuint program) const noexcept { glAttachShader(program, m_handle); }
    void detach(const GLuint program) const noexcept { glDetachShader(program, m_handle); }
    
private:
    void parseShaderFile(const std::string& filename);
    void compile() const;

    GLuint m_handle;
    std::string m_shaderSource;
};

//------------------------------------------------------------------------
