#pragma once

#include <GL/glew.h>

#include <memory>

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

template<GLenum TextureTarget>
concept SupportedGLTextureTarget = (
    TextureTarget == GL_TEXTURE_2D || TextureTarget == GL_TEXTURE_2D_ARRAY
);

//------------------------------------------------------------------------

template<GLenum TextureTarget, GLenum OrigTextureTarget>
concept ValidGLTextureViewTargetCombination = (
    SupportedGLTextureTarget<TextureTarget> && SupportedGLTextureTarget<OrigTextureTarget>
        && ((OrigTextureTarget == GL_TEXTURE_2D || OrigTextureTarget == GL_TEXTURE_2D_ARRAY)
            && (TextureTarget == GL_TEXTURE_2D || TextureTarget == GL_TEXTURE_2D_ARRAY))
);

//------------------------------------------------------------------------

template<GLenum TextureTarget = GL_TEXTURE_2D, GLenum OrigTextureTarget = GL_TEXTURE_2D_ARRAY>
requires ValidGLTextureViewTargetCombination<TextureTarget, OrigTextureTarget>
class TextureView
{
public:
    struct StorageDetails
    {
        GLuint handle;
        GLenum internalFormat;
        GLsizeiptr storageOffset;
    };

    TextureView(const StorageDetails& storageDetails, GLsizei levels)
        : m_storageDetails{storageDetails}
    {
        glGenTextures(1, &m_handle);
        glTextureView(
            m_handle,
            TextureTarget,
            m_storageDetails.handle,
            m_storageDetails.internalFormat,
            0, levels,
            m_storageDetails.storageOffset, 1
        );
    }

    ~TextureView() { glDeleteTextures(1, &m_handle); }

    TextureView(const TextureView&) = delete;
    TextureView& operator=(const TextureView&) = delete;
    TextureView(TextureView&&) = default;
    TextureView& operator=(TextureView&&) = default;

    [[nodiscard]] GLuint getGLHandle() const noexcept { return m_handle; }
    [[nodiscard]] bool isValid() const noexcept { return glIsTexture(m_handle) && glIsTexture(m_storageDetails.handle); }

private:
    GLuint m_handle;
    const StorageDetails m_storageDetails;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
