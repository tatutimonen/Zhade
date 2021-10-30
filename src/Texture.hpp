#pragma once

#include "Common.hpp"
#include "Util.hpp"

#include <GL/glew.h>
#include <SOIL.h>

#include <map>
#include <memory>

//------------------------------------------------------------------------

class Texture {
public:
    struct Settings {
        GLsizei levels = 1;
        GLenum internalFormat = GL_RGBA8;
        GLsizei width = 1;
        GLint border = 0;
        GLenum format = GL_RGBA;
        GLenum type = GL_UNSIGNED_BYTE;

        GLenum minFilter = GL_LINEAR_MIPMAP_LINEAR;
        GLenum magFilter = GL_LINEAR;
        GLenum wrapS = GL_CLAMP_TO_EDGE;

        Settings() = default;
        Settings(GLsizei width)
            : width{width}
        {}
    };

    virtual ~Texture() = default;

    virtual void setData(const void* data) = 0;

    virtual inline void bind() const = 0;
    virtual inline void unbind() const = 0;
};

//------------------------------------------------------------------------
