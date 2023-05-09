#include "Texture.hpp"

#include "ResourceManager.hpp"
#include "StbImageResource.hpp"

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

Texture::Texture(const glm::ivec2& dims, const Specification& spec, ResourceManagement management)
    : m_dims{dims},
      m_management{management}
{
    glCreateTextures(GL_TEXTURE_2D, 1, &m_name);
    glTextureStorage2D(m_name, spec.levels, spec.internalFormat, m_dims.x, m_dims.y);

    glTextureParameteri(m_name, GL_TEXTURE_MIN_FILTER, spec.minFilter);
    glTextureParameteri(m_name, GL_TEXTURE_MAG_FILTER, spec.magFilter);
    glTextureParameteri(m_name, GL_TEXTURE_WRAP_S, spec.wrapS);
    glTextureParameteri(m_name, GL_TEXTURE_WRAP_T, spec.wrapT);
    glTextureParameterf(m_name, GL_TEXTURE_MAX_ANISOTROPY, spec.anisotropy);

    if (spec.internalFormat == GL_DEPTH_COMPONENT32)  // Depth texture?
    {
        glTextureParameteri(m_name, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
        glTextureParameteri(m_name, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
    }

    m_handle = glGetTextureHandleARB(m_name);
    makeResident();
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
    if (m_management == ResourceManagement::RAII || glIsTexture(m_name) == GL_FALSE) [[unlikely]] return;
    glMakeTextureHandleNonResidentARB(m_handle);
    glDeleteTextures(1, &m_name);
}

//------------------------------------------------------------------------

Handle<Texture> Texture::fromFile(ResourceManager* mngr, std::string_view filename, const Specification& spec) noexcept
{
    const auto img = StbImageResource(filename);
    auto tex = mngr->createTexture(img.getDims(), spec);
    auto texPtr = mngr->get(tex);
    texPtr->setData(img.data());
    texPtr->generateMipmap();
    return tex;
}

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
