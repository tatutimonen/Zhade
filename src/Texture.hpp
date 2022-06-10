#pragma once

#include <GL/glew.h>

#include <cstdint>
#include <map>
#include <memory>

//------------------------------------------------------------------------

class Texture {
public:
    struct Settings {
        int32_t levels = 1;
        uint32_t internalFormat = GL_RGBA8;
        int32_t width = 1;
        int32_t border = 0;
        uint32_t format = GL_RGBA;
        uint32_t type = GL_UNSIGNED_BYTE;

        uint32_t minFilter = GL_LINEAR_MIPMAP_LINEAR;
        uint32_t magFilter = GL_LINEAR;
        uint32_t wrapS = GL_CLAMP_TO_EDGE;

        Settings() = default;
        Settings(GLsizei width)
            : width{width}
        {}
    };

    Texture() = default;
    virtual ~Texture() = default;
    Texture(const Texture&) = default;
    Texture(Texture&&) = default;
    Texture& operator=(const Texture&) = default;
    Texture& operator=(Texture&&) = default;

    virtual void setData(const void* data) noexcept = 0;
    virtual void setParameteri(uint32_t pname, int32_t param) const noexcept = 0;
    virtual const Settings& getSettings() const noexcept = 0;
    virtual uint32_t getHandle() const noexcept = 0;

    virtual void bind() const noexcept = 0;
    virtual void unbind() const noexcept = 0;
};

//------------------------------------------------------------------------
