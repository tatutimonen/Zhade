#pragma once

#include "Common.hpp"
#include "Util.hpp"

#include <GL/glew.h>
#include <SOIL.h>

#include <map>
#include <memory>

//------------------------------------------------------------------------

class ITexture {
public:
    struct Specification {
        GLsizei levels = 1;
        GLenum internalFormat = GL_RGBA8;
        GLsizei width;
        GLint border = 0;
        GLenum format = GL_RGBA;
        GLenum type = GL_UNSIGNED_BYTE;

        GLenum min_filter = GL_LINEAR_MIPMAP_LINEAR;
        GLenum mag_filter = GL_LINEAR;
        GLenum wrap_s = GL_CLAMP_TO_EDGE;

        Specification(GLsizei width)
            : width{width}
        {}
    };

    virtual ~ITexture() = default;

    virtual void setData(const void* data) = 0;

    virtual inline void bind() const = 0;
    virtual inline void unbind() const = 0;

    static const std::map<GLenum, GLenum> s_glFormatToSoilFormat;
};

//------------------------------------------------------------------------
