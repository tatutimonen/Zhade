#pragma once

#include "Handle.hpp"
#include "common.hpp"

#include <robin_hood.h>

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

class ResourceManager;

struct SamplerDescriptor
{
    GLenum wrapS = GL_REPEAT;
    GLenum wrapT = GL_REPEAT;
    GLenum magFilter = GL_LINEAR;
    GLenum minFilter = GL_LINEAR_MIPMAP_LINEAR;
    GLfloat anisotropy = 8.0f;
};

struct TextureDescriptor
{
    glm::ivec2 dims{256, 256};
    GLsizei levels = 8;
    GLenum internalFormat = GL_RGBA8;
    SamplerDescriptor sampler;
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

    [[nodiscard]] GLuint name() { return m_texture; }
    [[nodiscard]] GLuint64 handle() { return m_handle; }
    [[nodiscard]] const glm::ivec2& dims() { return m_dims; }

    void freeResources();
    void generateMipmap() { glGenerateTextureMipmap(m_texture); }
    void setData(const void* data, GLsizei depth = 0);

    [[nodiscard]] static Handle<Texture> fromFile(ResourceManager* mngr, const fs::path& path,
        TextureDescriptor desc = TextureDescriptor{});
    [[nodiscard]] static Handle<Texture> makeDefault(ResourceManager* mngr);

    static inline robin_hood::unordered_map<fs::path, Handle<Texture>> s_cache;

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
