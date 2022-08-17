#pragma once

#include <GL/glew.h>

#include <memory>

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

class TextureStorage;

//------------------------------------------------------------------------

template<GLenum TextureFormat>
concept IsSupportedGlTextureFormat = (
    TextureFormat == GL_TEXTURE_2D
        || TextureFormat == GL_TEXTURE_2D_ARRAY
);

//------------------------------------------------------------------------

template<GLenum TextureFormat, GLenum OrigTextureFormat>
concept IsValidViewOrigTargetCombination = (
    IsSupportedGlTextureFormat<TextureFormat> && IsSupportedGlTextureFormat<OrigTextureFormat>
        && ((OrigTextureFormat == GL_TEXTURE_2D || OrigTextureFormat == GL_TEXTURE_2D_ARRAY)
            && (TextureFormat == GL_TEXTURE_2D || TextureFormat == GL_TEXTURE_2D_ARRAY))
);

//------------------------------------------------------------------------

template<GLenum TextureFormat = GL_TEXTURE_2D, GLenum OrigTextureFormat = GL_TEXTURE_2D_ARRAY>
requires IsValidViewOrigTargetCombination<TextureFormat, OrigTextureFormat>
class TextureView
{
public:
    struct StorageDetails;

    TextureView(const StorageDetails& storageDetails)
        : m_storageDetails{storageDetails}
    {
        glGenTextures(1, &m_handle);
        const auto underlying = m_storageDetails.underlying.lock();
        const auto& settings = underlying->getSettings();
        glTextureView(
            m_handle,
            GL_TEXTURE_2D,
            underlying->getHandle(),
            settings.internalformat,
            0, settings.levels,
            m_storageDetails.offset, 1
        );
    }

    [[nodiscard]] GLuint getHandle() const noexcept { return m_handle; }

private:
    struct StorageDetails
    {
        std::weak_ptr<const TextureStorage> underlying;
        const GLsizeiptr offset;
    };

    GLuint m_handle;
    StorageDetails m_storageDetails;

    friend class TextureStorage;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
