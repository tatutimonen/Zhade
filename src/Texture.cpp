#include "Texture.hpp"

#include "ResourceManager.hpp"
#include "StbImageResource.hpp"

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

Texture::Texture(const glm::ivec2& dims, Desc desc, ResourceManagement management)
    : m_dims{dims},
      m_management{management}
{
    glCreateTextures(GL_TEXTURE_2D, 1, &m_name);
    glTextureStorage2D(m_name, desc.levels, desc.internalFormat, m_dims.x, m_dims.y);

    glTextureParameteri(m_name, GL_TEXTURE_MIN_FILTER, desc.minFilter);
    glTextureParameteri(m_name, GL_TEXTURE_MAG_FILTER, desc.magFilter);
    glTextureParameteri(m_name, GL_TEXTURE_WRAP_S, desc.wrapS);
    glTextureParameteri(m_name, GL_TEXTURE_WRAP_T, desc.wrapT);
    glTextureParameterf(m_name, GL_TEXTURE_MAX_ANISOTROPY, desc.anisotropy);

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
    if (m_management == ResourceManagement::MANUAL) [[likely]] return;
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

Handle<Texture> Texture::fromFile(ResourceManager* mngr, const fs::path& path, const Desc& desc) noexcept
{
    const auto img = StbImageResource(path);
    auto texHandle = mngr->createTexture(img.getDims(), desc);
    auto tex = mngr->get(texHandle);
    tex->setData(img.data());
    tex->generateMipmap();
    return texHandle;
}

//------------------------------------------------------------------------

Handle<Texture> Texture::makeDefault(ResourceManager* mngr) noexcept
{
    static constexpr Desc desc{
        .levels = 1,
        .internalFormat = GL_RGBA8,
        .minFilter = GL_NEAREST,
        .magFilter = GL_NEAREST,
        .wrapS = GL_CLAMP_TO_EDGE,
        .wrapT = GL_CLAMP_TO_EDGE,
        .anisotropy = 1.0f
    };
    static constexpr uint8_t data{0xff};

    auto texHandle = mngr->createTexture(glm::ivec2(1, 1), desc);
    mngr->get(texHandle)->setData(&data);
    return texHandle;
}

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
