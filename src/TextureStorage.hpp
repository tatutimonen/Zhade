#pragma once

#include "TextureView.hpp"

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <optional>
#include <string_view>

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

class TextureStorage : public std::enable_shared_from_this<TextureStorage>
{
public:
    /*
    Apple guidelines (maybe irrelevant):

    Optimal Data Formats and Types
    The best format and data type combinations to use for texture data are:

    GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV
    GL_BGRA, GL_UNSIGNED_SHORT_1_5_5_5_REV)
    GL_YCBCR_422_APPLE, GL_UNSIGNED_SHORT_8_8_REV_APPLE
    The combination GL_RGBA and GL_UNSIGNED_BYTE needs to be swizzled by many cards when the data is loaded, so it's not recommended.

    [https://developer.apple.com/library/archive/documentation/GraphicsImaging/Conceptual/OpenGL-MacProgGuide/opengl_texturedata/opengl_texturedata.html]
    */
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

    const Settings& getSettings() const noexcept { return m_settings; }
    void bindToUnit(const GLuint unit) const noexcept { glBindTextureUnit(unit, m_handle); }
    void generateMipmap() const noexcept { glGenerateTextureMipmap(m_handle); }
    bool fits(const uint32_t numTextures = 1) const noexcept { return numTextures < m_settings.depth - m_writeOffsetDepth; }

    std::optional<TextureView> setDataByOffset(const void* data, const GLsizeiptr offsetDepth) const noexcept;
    std::optional<TextureView> setDataFromFileByOffset(std::string_view filename, const GLsizeiptr offsetDepth) const noexcept;
    std::optional<TextureView> pushData(const void* data) const noexcept;
    std::optional<TextureView> pushDataFromFile(std::string_view filename) const noexcept;

private:
    std::optional<TextureView> setData(const void* data, const GLsizeiptr offsetDepth) const noexcept;

    GLuint m_handle;
    Settings m_settings;
    mutable GLsizeiptr m_writeOffsetDepth = 0;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
