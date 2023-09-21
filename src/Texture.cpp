#include "Texture.hpp"

#include "ResourceManager.hpp"
#include "StbImageResource.hpp"

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

Texture::Texture(TextureDescriptor desc)
    : m_dims{desc.dims},
      m_managed{desc.managed}
{
    glCreateTextures(GL_TEXTURE_2D, 1, &m_texture);
    glTextureStorage2D(m_texture, desc.levels, desc.internalFormat, m_dims.x, m_dims.y);

    glCreateSamplers(1, &m_sampler);
    glSamplerParameteri(m_sampler, GL_TEXTURE_WRAP_S, desc.sampler.wrapS);
    glSamplerParameteri(m_sampler, GL_TEXTURE_WRAP_T, desc.sampler.wrapT);
    glSamplerParameteri(m_sampler, GL_TEXTURE_MAG_FILTER, desc.sampler.magFilter);
    glSamplerParameteri(m_sampler, GL_TEXTURE_MIN_FILTER, desc.sampler.minFilter);
    glSamplerParameterf(m_sampler, GL_TEXTURE_MAX_ANISOTROPY, desc.sampler.anisotropy);
    if (desc.internalFormat == GL_DEPTH_COMPONENT32F)  // Depth texture?
    {
        glSamplerParameteri(m_sampler, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
        glSamplerParameteri(m_sampler, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
    }

    m_handle = glGetTextureSamplerHandleARB(m_texture, m_sampler);
    glMakeTextureHandleResidentARB(m_handle);
}

//------------------------------------------------------------------------

Texture::~Texture()
{
    if (m_managed) [[likely]] return;
    freeResources();
}

//------------------------------------------------------------------------

void Texture::freeResources() const noexcept
{
    glMakeTextureHandleNonResidentARB(m_handle);
    glDeleteTextures(1, &m_texture);
    glDeleteSamplers(1, &m_sampler);
}

//------------------------------------------------------------------------

Handle<Texture> Texture::fromFile(ResourceManager* mngr, const fs::path& path, TextureDescriptor desc) noexcept
{
    
    
    if (s_cache.contains(path) and mngr->get(s_cache[path]) != nullptr)
        return s_cache[path];

    const StbImageResource img{path};
    desc.dims = img.getDims();

    desc.managed = true;
    auto textureHandle = mngr->createTexture(desc);
    auto texture = mngr->get(textureHandle);
    texture->setData(img.data());
    texture->generateMipmap();

    s_cache[path] = textureHandle;

    return textureHandle;
}

//------------------------------------------------------------------------

Handle<Texture> Texture::makeDefault(ResourceManager* mngr) noexcept
{
    static constexpr TextureDescriptor desc{
        .dims = {1, 1},
        .levels = 1,
        .sampler = {
            .magFilter = GL_NEAREST,
            .minFilter = GL_NEAREST,
            .anisotropy = 1.0f
        }
    };
    static constexpr uint8_t data = 0xff;

    auto textureHandle = mngr->createTexture(desc);
    mngr->get(textureHandle)->setData(&data);
    return textureHandle;
}

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
