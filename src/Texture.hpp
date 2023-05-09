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

    void setData(const void* data) const noexcept
    {
        glTextureSubImage2D(m_name, 0, 0, 0, m_dims.x, m_dims.y, GL_RGBA, GL_UNSIGNED_BYTE, data);
    }

    [[nodiscard]] GLuint getHandle() const noexcept { return m_name; }
    [[nodiscard]] GLuint64 getTexHandle() const noexcept { return m_handle; }
    [[nodiscard]] const glm::ivec2& getDims() const noexcept { return m_dims; }

    void freeResources() const noexcept;
    void generateMipmap() const noexcept { glGenerateTextureMipmap(m_name); }
    void makeResident() const noexcept { glMakeTextureHandleResidentARB(m_handle); }

    static Handle<Texture> fromFile(ResourceManager* mngr, std::string_view filename, const Specification& spec = Specification{}) noexcept;

private:
    GLuint m_name = 0;
    GLuint64 m_handle;
    glm::ivec2 m_dims;
    ResourceManagement m_management = ResourceManagement::MANUAL;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
