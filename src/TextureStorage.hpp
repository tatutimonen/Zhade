#pragma once

#include "StbImageResource.hpp"
#include "TextureView.hpp"
#include "util.hpp"

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <mutex>
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

//------------------------------------------------------------------------

template<ValidGlTextureDataSourceType T>
static constexpr GLenum getGlTextureDataTypeOfPrimitive32BitType()
{
    if constexpr (std::is_same_v<T, uint8_t>)
        return GL_UNSIGNED_BYTE;
    else if (std::is_same_v<T, uint16_t>)
        return GL_UNSIGNED_SHORT;
    else if (std::is_same_v<T, uint32_t>)  // Assume 32-bit unsigned integers to be packed.
        return GL_UNSIGNED_INT_8_8_8_8_REV;
    else if (std::is_same_v<T, int32_t>)
        return GL_INT;
    else
        return GL_FLOAT;
}

//------------------------------------------------------------------------

class TextureStorage : public std::enable_shared_from_this<TextureStorage>
{
public:
    struct Settings
    {
        GLsizei levels;
        GLenum internalformat;
        GLsizei width;
        GLsizei height;
        GLsizei depth;

        GLenum format;
        GLenum type;

        GLenum minFilter;
        GLenum magFilter;
        GLenum wrapS;
        GLenum wrapT;

        static Settings makeDefaultOfSize(const glm::ivec4& dims)
        {
            return {
                .levels = dims.w,
                .internalformat = GL_RGBA8,
                .width = dims.x,
                .height = dims.y,
                .depth = dims.z,
                .format = GL_RGBA,
                .type = GL_UNSIGNED_BYTE,
                .minFilter = GL_LINEAR_MIPMAP_LINEAR,
                .magFilter = GL_LINEAR,
                .wrapS = GL_CLAMP_TO_EDGE,
                .wrapT = GL_CLAMP_TO_EDGE
            };
        }
    };

    TextureStorage(const Settings& settings = Settings::makeDefaultOfSize(glm::ivec4(256, 256, 128, 4)));
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
        std::lock_guard lg{m_mtx};
        return numTextures < m_settings.depth - m_writeOffsetDepth;
    }

    template<ValidGlTextureDataSourceType T, GLenum TextureTarget = GL_TEXTURE_2D>
    requires ValidGlTextureViewTargetCombination<TextureTarget, GL_TEXTURE_2D_ARRAY>
    [[nodiscard]] std::optional<TextureView<TextureTarget>> pushData(const T* data) const noexcept
    {
        std::lock_guard lg{m_mtx};

        if (!fits())
            return std::nullopt;

        return setData<T, TextureTarget>(data, m_writeOffsetDepth++);
    }

    template<StbImageDataFormat T = stbi_uc, GLenum TextureTarget = GL_TEXTURE_2D>
    requires ValidGlTextureViewTargetCombination<TextureTarget, GL_TEXTURE_2D_ARRAY>
    [[nodiscard]] std::optional<TextureView<TextureTarget>> pushDataFromFile(std::string_view filename) const noexcept
    {
        std::lock_guard lg{m_mtx};

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
        std::lock_guard lg{m_mtx};

        if (offsetDepth < 0 || m_settings.depth < offsetDepth)
            return std::nullopt;

        return setData<T, TextureTarget>(data, offsetDepth);
    }

    template<StbImageDataFormat T = stbi_uc, GLenum TextureTarget = GL_TEXTURE_2D>
    requires ValidGlTextureViewTargetCombination<TextureTarget, GL_TEXTURE_2D_ARRAY>
    [[nodiscard]] std::optional<TextureView<TextureTarget>> setDataFromFileByOffset(std::string_view filename,
        const GLsizeiptr offsetDepth) const noexcept
    {
        std::lock_guard lg{m_mtx};

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
        const auto view = TextureView<TextureTarget>({ .underlying = weak_from_this(), .offset = offsetDepth });
        return std::make_optional(view);
    }

    GLuint m_handle;
    Settings m_settings;
    mutable GLsizeiptr m_writeOffsetDepth = 0;
    mutable std::recursive_mutex m_mtx;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
