#pragma once

#include "StbImageResource.hpp"

extern "C" {
#include <GL/glew.h>
}

#include <string_view>

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

class Texture
{
public:
    struct Specification
    {
        GLsizei width, height;
        GLsizei levels = 8;
        GLenum internalFormat = GL_RGBA8;
        GLenum minFilter = GL_LINEAR_MIPMAP_LINEAR;
        GLenum magFilter = GL_LINEAR;
        GLenum wrapS = GL_CLAMP_TO_EDGE;
        GLenum wrapT = GL_CLAMP_TO_EDGE;
    };

    Texture(const Specification& spec);
    ~Texture();

    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;
    Texture(Texture&&) = default;
    Texture& operator=(Texture&&) = default;

    [[nodiscard]] GLuint getHandle() const noexcept { return m_handle; }
    [[nodiscard]] GLuint64 getTexHandle() const noexcept { return m_texHandle; }
    [[nodiscard]] const Specification& getSpec() const noexcept { return m_spec; }

    void generateMipmap() const noexcept { glGenerateTextureMipmap(m_handle); }
    void makeResident() const noexcept { glMakeTextureHandleResidentARB(m_texHandle); }

    void setData(const void* data) const noexcept
    {
        glTextureSubImage2D(m_handle, 0, 0, 0, m_spec.width, m_spec.height, GL_RGBA, GL_UNSIGNED_BYTE, data);
    }

    static Texture fromFile(std::string_view filename)
    {
        const auto img = StbImageResource(filename);
        const auto tex = Texture({ .width = img.getWidth(), .height = img.getHeight() });
        tex.setData(img.data());
        tex.generateMipmap();
        return tex;
    }

private:
    GLuint m_handle;
    GLuint64 m_texHandle;
    Specification m_spec;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
