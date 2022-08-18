#pragma once

#include <GL/glew.h>

#include <memory>

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

class TextureStorage;

//------------------------------------------------------------------------

template<GLenum TextureTarget>
concept SupportedGlTextureTarget = (
    TextureTarget == GL_TEXTURE_2D || TextureTarget == GL_TEXTURE_2D_ARRAY
);

//------------------------------------------------------------------------

template<GLenum TextureTarget, GLenum OrigTextureTarget>
concept ValidGlTextureViewTargetCombination = (
    SupportedGlTextureTarget<TextureTarget> && SupportedGlTextureTarget<OrigTextureTarget>
        && ((OrigTextureTarget == GL_TEXTURE_2D || OrigTextureTarget == GL_TEXTURE_2D_ARRAY)
            && (TextureTarget == GL_TEXTURE_2D || TextureTarget == GL_TEXTURE_2D_ARRAY))
);

//------------------------------------------------------------------------

template<GLenum TextureTarget = GL_TEXTURE_2D, GLenum OrigTextureTarget = GL_TEXTURE_2D_ARRAY>
requires ValidGlTextureViewTargetCombination<TextureTarget, OrigTextureTarget>
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
