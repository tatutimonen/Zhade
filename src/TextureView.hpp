#pragma once

#include <GL/glew.h>

#include <memory>

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

template<GLenum InternalFormat>
requires SupportedGlInternalFormat<InternalFormat>
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

template<GLenum ViewInternalFormat>
concept ValidGlInternalFormat = (
    ViewInternalFormat == GL_RGBA8
);

//------------------------------------------------------------------------

template<GLenum InternalFormat = GL_RGBA8, GLenum TextureTarget = GL_TEXTURE_2D, GLenum OrigTextureTarget = GL_TEXTURE_2D_ARRAY>
requires ValidGlInternalFormat<InternalFormat> && ValidGlTextureViewTargetCombination<TextureTarget, OrigTextureTarget>
class TextureView
{
public:
    struct StorageDetails
    {
        GLuint handle;
        GLsizeiptr storageOffset;
    };

    TextureView(const StorageDetails& storageDetails)
        : m_storageDetails{storageDetails}
    {
        glGenTextures(1, &m_handle);
        glTextureView(
            m_handle,
            GL_TEXTURE_2D,
            m_storageDetails.handle,
            InternalFormat,
            0, settings.levels,
            m_storageDetails.offset, 1
        );
    }

    ~TextureView() { glDeleteTextures(1, &m_handle); }

    TextureView(const TextureView&) = delete;
    TextureView& operator=(const TextureView&) = delete;
    TextureView(TextureView&&) = default;
    TextureView& operator=(TextureView&&) = default;

    [[nodiscard]] GLuint getHandle() const noexcept { return m_handle; }
    [[nodiscard]] bool isValid() const noexcept { return glIsTexture(m_storageDetails.handle) && glIsTexture(m_handle); }

private:
    GLuint m_handle;
    const StorageDetails m_storageDetails;

    friend class TextureStorage;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
