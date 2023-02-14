#pragma once

#include "StbImageResource.hpp"
#include "TextureView.hpp"
#include "util.hpp"

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <optional>
#include <string_view>

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

template<typename T>
concept ValidGLTextureDataSourceType = (
    StbImageDataFormat<T> || std::unsigned_integral<T> && sizeof(T) <= 4 || std::same_as<T, int32_t> || std::same_as<T, float>
);

template<GLenum InternalFormat>
concept SupportedGLInternalFormat = (
    InternalFormat == GL_RGBA8 || InternalFormat == GL_DEPTH_COMPONENT32
);

//------------------------------------------------------------------------

template<ValidGLTextureDataSourceType T>
inline constexpr GLenum textureDataSourceType2GLenum()
{
    if constexpr (std::same_as<T, uint8_t>)
        return GL_UNSIGNED_BYTE;
    else if (std::same_as<T, uint16_t>)
        return GL_UNSIGNED_SHORT;
    else if (std::same_as<T, uint32_t>)
        return GL_UNSIGNED_INT_8_8_8_8_REV;
    else if (std::same_as<T, int32_t>)
        return GL_INT;
    else if (std::same_as<T, float>)
        return GL_FLOAT;
}

template<GLenum InternalFormat>
requires SupportedGLInternalFormat<InternalFormat>
inline constexpr GLenum textureInternalFormat2Format()
{
    if constexpr (InternalFormat == GL_RGBA8)
        return GL_RGBA;
    else if (InternalFormat == GL_DEPTH_COMPONENT32)
        return GL_DEPTH_COMPONENT;
}

//------------------------------------------------------------------------

template<GLenum InternalFormat = GL_RGBA8, ValidGLTextureDataSourceType T = stbi_uc>
requires SupportedGLInternalFormat<InternalFormat>
class TextureStorage
{
public:
    struct Settings
    {
        GLsizei width, height, depth, levels;
        GLenum minFilter, magFilter, wrapS, wrapT;

        static Settings makeDefault(const glm::ivec4& dims)
        {
            if constexpr (InternalFormat == GL_RGBA8)
                return {
                    .width = dims.x, .height = dims.y, .depth = dims.z, .levels = dims.w,
                    .minFilter = GL_LINEAR_MIPMAP_LINEAR, .magFilter = GL_LINEAR,
                    .wrapS = GL_CLAMP_TO_EDGE, .wrapT = GL_CLAMP_TO_EDGE
                };
            else if (InternalFormat == GL_DEPTH_COMPONENT32)
                return {
                    .width = dims.x, .height = dims.y, .depth = dims.z, .levels = dims.w,
                    .minFilter = GL_LINEAR, .magFilter = GL_LINEAR,
                    .wrapS = GL_CLAMP_TO_EDGE, .wrapT = GL_CLAMP_TO_EDGE
                };
        }
    };

    TextureStorage(const Settings& settings = Settings::makeDefault(glm::ivec4(256, 256, 128, 4)))
        : m_settings{settings}
    {
        glCreateTextures(GL_TEXTURE_2D_ARRAY, 1, &m_handle);
        glTextureStorage3D(
            m_handle,
            m_settings.levels, InternalFormat,
            m_settings.width, m_settings.height, m_settings.depth
        );
        glTextureParameteri(m_handle, GL_TEXTURE_MIN_FILTER, m_settings.minFilter);
        glTextureParameteri(m_handle, GL_TEXTURE_MAG_FILTER, m_settings.magFilter);
        glTextureParameteri(m_handle, GL_TEXTURE_WRAP_S, m_settings.wrapS);
        glTextureParameteri(m_handle, GL_TEXTURE_WRAP_T, m_settings.wrapT);

        // Depth texture?
        if constexpr (InternalFormat == GL_DEPTH_COMPONENT32)
        {
            glTextureParameteri(m_handle, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
            glTextureParameteri(m_handle, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
        }
    }

    TextureStorage(GLint width, GLint height, GLint depth, GLint levels)
    {
        TextureStorage(Settings::makeDefault(glm::ivec4(width, height, depth, levels)));
    }
    
    ~TextureStorage()
    {
        glDeleteTextures(1, &m_handle);
    }

    TextureStorage(const TextureStorage&) = delete;
    TextureStorage& operator=(const TextureStorage&) = delete;
    TextureStorage(TextureStorage&&) = default;
    TextureStorage& operator=(TextureStorage&&) = default;

    [[nodiscard]] GLuint getGLHandle() const noexcept { return m_handle; }
    [[nodiscard]] const Settings& getSettings() const noexcept { return m_settings; }

    void bindToUnit(GLuint unit) const noexcept { glBindTextureUnit(unit, m_handle); }
    void generateMipmap() const noexcept { glGenerateTextureMipmap(m_handle); }
    
    [[nodiscard]] bool fits(uint32_t numTextures = 1) const noexcept
    {
        return numTextures < m_settings.depth - m_writeOffsetDepth;
    }

    template<GLenum TextureTarget>
    using ViewOpt = std::optional<TextureView<TextureTarget>>;

    template<GLenum TextureTarget = GL_TEXTURE_2D>
    requires ValidGLTextureViewTargetCombination<TextureTarget, GL_TEXTURE_2D_ARRAY>
    [[nodiscard]] ViewOpt<TextureTarget> pushData(const T* data) const noexcept
    {
        if (!fits()) [[unlikely]]
            return std::nullopt;

        return setData<TextureTarget>(data, m_writeOffsetDepth++);
    }

    template< GLenum TextureTarget = GL_TEXTURE_2D>
    requires ValidGLTextureViewTargetCombination<TextureTarget, GL_TEXTURE_2D_ARRAY>
    [[nodiscard]] ViewOpt<TextureTarget> setDataByOffset(const T* data, GLsizeiptr offsetDepth) const noexcept
    {
        if (offsetDepth < 0 || m_settings.depth < offsetDepth) [[unlikely]]
            return std::nullopt;

        return setData<TextureTarget>(data, offsetDepth);
    }

    template<GLenum TextureTarget = GL_TEXTURE_2D>
    requires ValidGLTextureViewTargetCombination<TextureTarget, GL_TEXTURE_2D_ARRAY> && StbImageDataFormat<T>
    [[nodiscard]] ViewOpt<TextureTarget> pushDataFromFile(std::string_view filename) const noexcept
    {
        if (!fits()) [[unlikely]]
            return std::nullopt;

        const auto image = StbImageResource<T>(filename);
        return setData<TextureTarget>(image.data(), m_writeOffsetDepth++);
    }

    template<GLenum TextureTarget = GL_TEXTURE_2D>
    requires ValidGLTextureViewTargetCombination<TextureTarget, GL_TEXTURE_2D_ARRAY> && StbImageDataFormat<T>
    [[nodiscard]] ViewOpt<TextureTarget> setDataFromFileByOffset(std::string_view filename, GLsizeiptr offsetDepth) const noexcept
    {
        if (offsetDepth < 0 || m_settings.depth < offsetDepth) [[unlikely]]
            return std::nullopt;

        const auto image = StbImageResource<T>(filename);
        return setData<TextureTarget>(image.data(), offsetDepth);
    }

private:
    template<GLenum TextureTarget = GL_TEXTURE_2D>
    requires ValidGLTextureViewTargetCombination<TextureTarget, GL_TEXTURE_2D_ARRAY>
    [[nodiscard]] ViewOpt<TextureTarget> setData(const T* data, GLsizeiptr offsetDepth) const noexcept
    {
        glTextureSubImage3D(
            m_handle, 0,
            0, 0, offsetDepth,
            m_settings.width, m_settings.height, 1,
            textureInternalFormat2Format<InternalFormat>(),
            textureDataSourceType2GLenum<T>(),
            data
        );
        return std::make_optional<TextureView<TextureTarget>>(
            TextureView<>::StorageDetails{
                .handle = m_handle, .internalFormat = InternalFormat, .storageOffset = offsetDepth
            },
            m_settings.levels
        );
    }

    GLuint m_handle;
    Settings m_settings;
    mutable GLsizeiptr m_writeOffsetDepth = 0;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
