#include "TextureStorage.hpp"

#include "StbImageResource.hpp"

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

TextureStorage::TextureStorage(const Settings& settings)
    : m_settings{settings}
{
    glCreateTextures(GL_TEXTURE_2D_ARRAY, 1, &m_handle);
    glTextureStorage3D(
        m_handle,
        m_settings.levels,
        m_settings.internalformat,
        m_settings.width,
        m_settings.height,
        m_settings.depth
    );
    glTextureParameteri(m_handle, GL_TEXTURE_MIN_FILTER, m_settings.minFilter);
    glTextureParameteri(m_handle, GL_TEXTURE_MAG_FILTER, m_settings.magFilter);
    glTextureParameteri(m_handle, GL_TEXTURE_WRAP_S, m_settings.wrapS);
    glTextureParameteri(m_handle, GL_TEXTURE_WRAP_T, m_settings.wrapT);
}

//------------------------------------------------------------------------

TextureStorage::TextureStorage(const GLint width, const GLint height, const GLint depth, const GLint levels)
{
    TextureStorage(Settings::makeDefaultOfSize(glm::ivec4(width, height, depth, levels)));
}

//------------------------------------------------------------------------

TextureStorage::~TextureStorage()
{
    glDeleteTextures(1, &m_handle);
}

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
