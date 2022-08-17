#pragma once

#include "TextureView.hpp"
#include "StbImageResource.hpp"

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <mutex>
#include <optional>
#include <string_view>

//------------------------------------------------------------------------

namespace Zhade
{

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

        static Settings makeDefaultOfSize(const glm::ivec3& dims)
        {
            return {
                .levels = 4,
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

    TextureStorage(const Settings& settings = Settings::makeDefaultOfSize(glm::ivec3(256, 256, 128)));
    ~TextureStorage();

    TextureStorage(const TextureStorage&) = delete;
    TextureStorage(TextureStorage&&) = default;
    TextureStorage& operator=(const TextureStorage&) = delete;
    TextureStorage& operator=(TextureStorage&&) = default;

    [[nodiscard]] GLuint getHandle() const noexcept { return m_handle; }
    [[nodiscard]] const Settings& getSettings() const noexcept { return m_settings; }
    void bindToUnit(const GLuint unit) const noexcept { glBindTextureUnit(unit, m_handle); }
    void generateMipmap() const noexcept { glGenerateTextureMipmap(m_handle); }
    
    [[nodiscard]] bool fits(const uint32_t numTextures = 1) const noexcept
    {
        return numTextures < m_settings.depth - m_writeOffsetDepth;
    }

    template<GLenum TextureFormat = GL_TEXTURE_2D>
    requires IsValidViewOrigTargetCombination<TextureFormat, GL_TEXTURE_2D_ARRAY>
    [[nodiscard]] std::optional<TextureView<TextureFormat>> setDataByOffset(const void* data,
        const GLsizeiptr offsetDepth) const noexcept
    {
        if (offsetDepth < 0 || m_settings.depth < offsetDepth)
            return std::nullopt;

        return setData<TextureFormat>(data, offsetDepth);
    }

    template<GLenum TextureFormat = GL_TEXTURE_2D>
    requires IsValidViewOrigTargetCombination<TextureFormat, GL_TEXTURE_2D_ARRAY>
    [[nodiscard]] std::optional<TextureView<TextureFormat>> setDataFromFileByOffset(std::string_view filename,
        const GLsizeiptr offsetDepth) const noexcept
    {
        if (offsetDepth < 0 || m_settings.depth < offsetDepth)
            return std::nullopt;

        const auto image = StbImageResource(filename);
        return setData<TextureFormat>(image.data(), offsetDepth);
    }

    template<GLenum TextureFormat = GL_TEXTURE_2D>
    requires IsValidViewOrigTargetCombination<TextureFormat, GL_TEXTURE_2D_ARRAY>
    [[nodiscard]] std::optional<TextureView<TextureFormat>> pushData(const void* data) const noexcept
    {
        if (!fits())
            return std::nullopt;

        return setData<TextureFormat>(data, m_writeOffsetDepth++);
    }

    template<GLenum TextureFormat = GL_TEXTURE_2D>
    requires IsValidViewOrigTargetCombination<TextureFormat, GL_TEXTURE_2D_ARRAY>
    [[nodiscard]] std::optional<TextureView<TextureFormat>> pushDataFromFile(std::string_view filename) const noexcept
    {
        if (!fits())
            return std::nullopt;

        const auto image = StbImageResource(filename);
        return setData<TextureFormat>(image.data(), m_writeOffsetDepth++);
    }

private:
    template<GLenum TextureFormat = GL_TEXTURE_2D>
    [[nodiscard]] std::optional<TextureView<TextureFormat>> setData(const void* data,
        const GLsizeiptr offsetDepth) const noexcept
    {
        glTextureSubImage3D(
            m_handle, 0,
            0, 0, offsetDepth,
            m_settings.width, m_settings.height, 1,
            m_settings.format,
            m_settings.type,
            data
        );
        const auto view = TextureView<TextureFormat>({ .underlying = weak_from_this(), .offset = offsetDepth });
        return std::make_optional(view);
    }

    GLuint m_handle;
    Settings m_settings;
    mutable GLsizeiptr m_writeOffsetDepth = 0;
    mutable std::mutex m_mtx;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
