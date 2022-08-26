#pragma once

#include "StbImageResource.hpp"
#include "TextureView.hpp"

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <optional>
#include <string_view>
#include <type_traits>

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

template<typename T>
concept ValidGlTextureDataSourceType = (
    std::is_unsigned_v<T> && sizeof(T) <= 4 || std::is_same_v<T, int32_t> || std::is_same_v<T, float>
);

template<GLenum InternalFormat>
concept SupportedGlInternalFormat = (
    InternalFormat == GL_RGBA8
        || InternalFormat == GL_DEPTH_COMPONENT32
);

//------------------------------------------------------------------------

template<ValidGlTextureDataSourceType T>
static constexpr GLenum getGlTextureDataTypeOfSourceType()
{
    if constexpr (std::is_same_v<T, uint8_t>)
        return GL_UNSIGNED_BYTE;
    else if (std::is_same_v<T, uint16_t>)
        return GL_UNSIGNED_SHORT;
    else if (std::is_same_v<T, uint32_t>)
        return GL_UNSIGNED_INT_8_8_8_8_REV;  // Assume 32-bit unsigned integers to be packed.
    else if (std::is_same_v<T, int32_t>)
        return GL_INT;
    else
        return GL_FLOAT;
}

template<GLenum InternalFormat>
requires SupportedGlInternalFormat<InternalFormat>
static constexpr GLenum getGlTextureFormatOfInternalFormat()
{
    switch (InternalFormat)
    {
    case GL_RGBA8:
                               return GL_RGBA;
    case GL_DEPTH_COMPONENT32:
                               return GL_DEPTH_COMPONENT;
    }
}

//------------------------------------------------------------------------

template<GLenum InternalFormat = GL_RGBA8>
requires SupportedGlInternalFormat<InternalFormat>
class TextureStorage
{
public:
    struct Settings
    {
        GLsizei width, height, depth, levels;
        GLenum type, minFilter, magFilter, wrapS, wrapT;

        static Settings makeDefaultOfSize(const glm::ivec4& dims)
        {
            return {
                .width = dims.x,
                .height = dims.y,
                .depth = dims.z,
                .levels = dims.w,
                .format = getGlTextureFormatOfInternalFormat<InternalFormat>(),
                .type = GL_UNSIGNED_BYTE,
                .minFilter = GL_LINEAR_MIPMAP_LINEAR,
                .magFilter = GL_LINEAR,
                .wrapS = GL_CLAMP_TO_EDGE,
                .wrapT = GL_CLAMP_TO_EDGE
            };
        }
    };

    TextureStorage(const Settings& settings = Settings::makeDefaultOfSize(glm::ivec4(256, 256, 128, 4)))
        : m_settings{settings}
    {
        glCreateTextures(GL_TEXTURE_2D_ARRAY, 1, &m_handle);
        glTextureStorage3D(
            m_handle,
            m_settings.levels,
            InternalFormat,
            m_settings.width,
            m_settings.height,
            m_settings.depth
        );
        glTextureParameteri(m_handle, GL_TEXTURE_MIN_FILTER, m_settings.minFilter);
        glTextureParameteri(m_handle, GL_TEXTURE_MAG_FILTER, m_settings.magFilter);
        glTextureParameteri(m_handle, GL_TEXTURE_WRAP_S, m_settings.wrapS);
        glTextureParameteri(m_handle, GL_TEXTURE_WRAP_T, m_settings.wrapT);
    }

    TextureStorage(const GLint width, const GLint height, const GLint depth, const GLint levels);
    ~TextureStorage();

    TextureStorage(const TextureStorage&) = delete;
    TextureStorage& operator=(const TextureStorage&) = delete;
    TextureStorage(TextureStorage&&) = default;
    TextureStorage& operator=(TextureStorage&&) = default;

    [[nodiscard]] GLuint getHandle() const noexcept { return m_handle; }
    [[nodiscard]] const Settings& getSettings() const noexcept { return m_settings; }
    void bindToUnit(const GLuint unit) const noexcept { glBindTextureUnit(unit, m_handle); }
    void generateMipmap() const noexcept { glGenerateTextureMipmap(m_handle); }
    
    [[nodiscard]] bool fits(const uint32_t numTextures = 1) const noexcept
    {
        return numTextures < m_settings.depth - m_writeOffsetDepth;
    }

    template<ValidGlTextureDataSourceType T, GLenum TextureTarget = GL_TEXTURE_2D>
    requires ValidGlTextureViewTargetCombination<TextureTarget, GL_TEXTURE_2D_ARRAY>
    [[nodiscard]] std::optional<TextureView<TextureTarget>> pushData(const T* data) const noexcept
    {
        if (!fits())
            return std::nullopt;

        return setData<T, TextureTarget>(data, m_writeOffsetDepth++);
    }

    template<StbImageDataFormat T = stbi_uc, GLenum TextureTarget = GL_TEXTURE_2D>
    requires ValidGlTextureViewTargetCombination<TextureTarget, GL_TEXTURE_2D_ARRAY>
    [[nodiscard]] std::optional<TextureView<TextureTarget>> pushDataFromFile(std::string_view filename) const noexcept
    {
        if (!fits())
            return std::nullopt;

        const auto image = StbImageResource<T>(filename);
        return setData<T, TextureTarget>(image.data(), m_writeOffsetDepth++);
    }

    template<ValidGlTextureDataSourceType T, GLenum TextureTarget = GL_TEXTURE_2D>
    requires ValidGlTextureViewTargetCombination<TextureTarget, GL_TEXTURE_2D_ARRAY>
    [[nodiscard]] std::optional<TextureView<TextureTarget>> setDataByOffset(const T* data,
        const GLsizeiptr offsetDepth) const noexcept
    {
        if (offsetDepth < 0 || m_settings.depth < offsetDepth)
            return std::nullopt;

        return setData<T, TextureTarget>(data, offsetDepth);
    }

    template<StbImageDataFormat T = stbi_uc, GLenum TextureTarget = GL_TEXTURE_2D>
    requires ValidGlTextureViewTargetCombination<TextureTarget, GL_TEXTURE_2D_ARRAY>
    [[nodiscard]] std::optional<TextureView<TextureTarget>> setDataFromFileByOffset(std::string_view filename,
        const GLsizeiptr offsetDepth) const noexcept
    {
        if (offsetDepth < 0 || m_settings.depth < offsetDepth)
            return std::nullopt;

        const auto image = StbImageResource<T>(filename);
        return setData<TextureTarget>(image.data(), offsetDepth);
    }

private:
    template<ValidGlTextureDataSourceType T, GLenum TextureTarget = GL_TEXTURE_2D>
    requires ValidGlTextureViewTargetCombination<TextureTarget, GL_TEXTURE_2D_ARRAY>
    [[nodiscard]] std::optional<TextureView<TextureTarget>> setData(const T* data, const GLsizeiptr offsetDepth) const noexcept
    {
        const GLenum textureDataType = getGlTextureDataTypeOfPrimitive32BitType<T>();
        glTextureSubImage3D(
            m_handle, 0,
            0, 0, offsetDepth,
            m_settings.width, m_settings.height, 1,
            m_settings.format,
            getGlTextureDataTypeOfPrimitive32BitType<T>(),
            data
        );
        const auto view = TextureView<TextureTarget>({ .handle = m_handle, .storageOffset = offsetDepth });
        return std::make_optional(view);
    }

    GLuint m_handle;
    Settings m_settings;
    mutable GLsizeiptr m_writeOffsetDepth = 0;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
