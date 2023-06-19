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
    glCreateTextures(GL_TEXTURE_2D, 1, &m_name);
    glTextureStorage2D(m_name, desc.levels, desc.internalFormat, m_dims.x, m_dims.y);

    const SamplerDescriptor& sampler = desc.sampler;
    glTextureParameteri(m_name, GL_TEXTURE_WRAP_S, sampler.wrapS);
    glTextureParameteri(m_name, GL_TEXTURE_WRAP_T, sampler.wrapT);
    glTextureParameteri(m_name, GL_TEXTURE_MAG_FILTER, sampler.magFilter);
    glTextureParameteri(m_name, GL_TEXTURE_MIN_FILTER, sampler.minFilter);
    glTextureParameterf(m_name, GL_TEXTURE_MAX_ANISOTROPY, sampler.anisotropy);

    if (desc.internalFormat == GL_DEPTH_COMPONENT32)  // Depth texture?
    {
        glTextureParameteri(m_name, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
        glTextureParameteri(m_name, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
    }

    m_handle = glGetTextureHandleARB(m_name);
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
    if (glIsTexture(m_name) == GL_FALSE) [[unlikely]] return;
    glMakeTextureHandleNonResidentARB(m_handle);
    glDeleteTextures(1, &m_name);
}

//------------------------------------------------------------------------

Handle<Texture> Texture::fromFile(ResourceManager* mngr, const fs::path& path, TextureDescriptor desc) noexcept
{
    const auto img = StbImageResource(path);
    desc.dims = img.getDims();
    
    desc.managed = true;
    auto texHandle = mngr->createTexture(desc);
    
    auto tex = mngr->get(texHandle);
    tex->setData(img.data());
    tex->generateMipmap();
    
    return texHandle;
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

    auto texHandle = mngr->createTexture(desc);
    mngr->get(texHandle)->setData(&data);
    return texHandle;
}

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
