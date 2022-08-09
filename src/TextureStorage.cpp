#include "TextureStorage.hpp"

#include "util.hpp"

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

TextureStorage::~TextureStorage()
{
    glDeleteTextures(1, &m_handle);
}

//------------------------------------------------------------------------

std::optional<TextureView> TextureStorage::setDataByOffset(const void* data, const GLsizeiptr offsetDepth) const noexcept
{
    if (offsetDepth < 0 || m_settings.depth < offsetDepth)
        return std::nullopt;

    return setData(data, offsetDepth);
}

//------------------------------------------------------------------------

std::optional<TextureView> TextureStorage::setDataFromFileByOffset(std::string_view filename, const GLsizeiptr offsetDepth) const noexcept
{
    if (offsetDepth < 0 || m_settings.depth < offsetDepth)
        return std::nullopt;

    const auto image = util::StbImageResource<>(filename);
    return setData(image.data(), offsetDepth);
}

//------------------------------------------------------------------------

std::optional<TextureView> TextureStorage::pushData(const void* data) const noexcept
{
    if (!fits())
        return std::nullopt;

    return setData(data, m_writeOffsetDepth++);
}

//------------------------------------------------------------------------

std::optional<TextureView> TextureStorage::pushDataFromFile(std::string_view filename) const noexcept
{
    if (!fits())
        return std::nullopt;

    const auto image = util::StbImageResource<>(filename);
    return setData(image.data(), m_writeOffsetDepth++);
}

//------------------------------------------------------------------------

std::optional<TextureView> TextureStorage::setData(const void* data, const GLsizeiptr offsetDepth) const noexcept
{
    glTextureSubImage3D(
        m_handle, 0,
        0, 0, offsetDepth,
        m_settings.width, m_settings.height, 1,
        m_settings.format,
        m_settings.type,
        data
    );

    const auto view = TextureView({
        .underlying = weak_from_this(),
        .offset = offsetDepth
    });
    glTextureView(
        view.getHandle(),
        GL_TEXTURE_2D,
        m_handle,
        m_settings.internalformat,
        0, m_settings.levels,
        offsetDepth, 1
    );

    return std::make_optional(view);
}

//------------------------------------------------------------------------
