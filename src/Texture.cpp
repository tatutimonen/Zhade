#include "Texture.hpp"

#include <iostream>

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

Texture::Texture(const Specification& spec)
    : m_spec{spec}
{
    glCreateTextures(GL_TEXTURE_2D, 1, &m_handle);
    glTextureStorage2D(m_handle, m_spec.levels, m_spec.internalFormat, m_spec.width, m_spec.height);

    glTextureParameteri(m_handle, GL_TEXTURE_MIN_FILTER, m_spec.minFilter);
    glTextureParameteri(m_handle, GL_TEXTURE_MAG_FILTER, m_spec.magFilter);
    glTextureParameteri(m_handle, GL_TEXTURE_WRAP_S, m_spec.wrapS);
    glTextureParameteri(m_handle, GL_TEXTURE_WRAP_T, m_spec.wrapT);

    if (m_spec.internalFormat == GL_DEPTH_COMPONENT32)  // Depth texture?
    {
        glTextureParameteri(m_handle, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
        glTextureParameteri(m_handle, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
    }

    m_texHandle = glGetTextureHandleARB(m_handle);
    makeResident();
}

//------------------------------------------------------------------------

Texture::~Texture()
{
    glMakeTextureHandleNonResidentARB(m_texHandle);
    glDeleteTextures(1, &m_handle);
}

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
