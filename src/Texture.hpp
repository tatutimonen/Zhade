#pragma once

#include "Handle.hpp"
#include "common.hpp"

#include <glm/glm.hpp>
extern "C" {
#include <GL/glew.h>
}

#include <string_view>

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

class ResourceManager;

//------------------------------------------------------------------------

class Texture
{
public:
    struct Specification
    {
        GLsizei levels = 8;
        GLenum internalFormat = GL_RGBA8;
        GLenum minFilter = GL_LINEAR_MIPMAP_LINEAR;
        GLenum magFilter = GL_LINEAR;
        GLenum wrapS = GL_CLAMP_TO_EDGE;
        GLenum wrapT = GL_CLAMP_TO_EDGE;
        GLfloat anisotropy = 8.0f;
    };

    Texture() = default;
    Texture(const glm::ivec2& dims, const Specification& spec, ResourceManagement management);
    ~Texture();

    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;
    Texture(Texture&&) = default;
    Texture& operator=(Texture&&) = default;

    [[nodiscard]] GLuint getHandle() const noexcept { return m_handle; }
    [[nodiscard]] GLuint64 getTexHandle() const noexcept { return m_texHandle; }
    [[nodiscard]] const glm::ivec2& getDims() const noexcept { return m_dims; }

    void generateMipmap() const noexcept { glGenerateTextureMipmap(m_handle); }
    void makeResident() const noexcept { glMakeTextureHandleResidentARB(m_texHandle); }

    void setData(const void* data) const noexcept
    {
        glTextureSubImage2D(m_handle, 0, 0, 0, m_dims.x, m_dims.y, GL_RGBA, GL_UNSIGNED_BYTE, data);
    }

    static Texture fromFile([[maybe_unused]] ResourceManager* mngr, std::string_view filename, const Specification& spec = Specification{}) noexcept;

private:
    void freeResources() const noexcept;

    GLuint m_handle = 0;
    GLuint64 m_texHandle;
    glm::ivec2 m_dims;
    ResourceManagement m_management = ResourceManagement::MANUAL;

    friend class ResourceManager;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
