#pragma once

#include "Handle.hpp"
#include "common.hpp"

#include <glm/glm.hpp>
#include <robin_hood.h>
extern "C" {
#include <GL/glew.h>
}

#include <mutex>

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

class ResourceManager;

struct SamplerDescriptor
{
    GLenum wrapS = GL_CLAMP_TO_EDGE;
    GLenum wrapT = GL_CLAMP_TO_EDGE;
    GLenum magFilter = GL_LINEAR;
    GLenum minFilter = GL_LINEAR_MIPMAP_LINEAR;
    GLfloat anisotropy = 8.0f;
};

struct TextureDescriptor
{
    glm::ivec2 dims{256, 256};
    GLsizei levels = 8;
    GLenum internalFormat = GL_RGBA8;
    SamplerDescriptor sampler{};
    bool managed = true;
};

//------------------------------------------------------------------------

class Texture
{
public:
    Texture() = default;
    explicit Texture(TextureDescriptor desc);
    ~Texture();

    Texture(const Texture&) = default;
    Texture& operator=(const Texture&) = default;
    Texture(Texture&&) = delete;
    Texture& operator=(Texture&&) = delete;

    void setData(const void* data) const noexcept
    {
        glTextureSubImage2D(m_texture, 0, 0, 0, m_dims.x, m_dims.y, GL_RGBA, GL_UNSIGNED_BYTE, data);
    }

    [[nodiscard]] GLuint getName() const noexcept { return m_texture; }
    [[nodiscard]] GLuint64 getHandle() const noexcept { return m_handle; }
    [[nodiscard]] const glm::ivec2& getDims() const noexcept { return m_dims; }

    void freeResources() const noexcept;
    void generateMipmap() const noexcept { glGenerateTextureMipmap(m_texture); }

    static Handle<Texture> fromFile(ResourceManager* mngr, const fs::path& path, TextureDescriptor desc = TextureDescriptor{}) noexcept;
    static Handle<Texture> makeDefault(ResourceManager* mngr) noexcept;

    static inline robin_hood::unordered_map<fs::path, Handle<Texture>> s_cache;
    static inline std::mutex s_mtx;

private:
    GLuint m_texture = 0;
    GLuint m_sampler = 0;
    GLuint64 m_handle = 0;
    glm::ivec2 m_dims{};
    bool m_managed = true;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
