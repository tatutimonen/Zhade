#include "Texture2D.hpp"

//------------------------------------------------------------------------

Texture2D::Texture2D(const std::shared_ptr<Settings>& settings)
    : m_settings{settings}
{
    setupTexture();
}

//------------------------------------------------------------------------

Texture2D::Texture2D(const std::string& filename)
    : m_settings{std::make_shared<Settings>()}
{
    stbi_set_flip_vertically_on_load(1);
    uint8_t* imageData = stbi_load(filename.c_str(), &m_settings->width, &m_settings->height, nullptr, 4);
    setupTexture();
    setData(imageData);
    stbi_image_free(imageData);
}

//------------------------------------------------------------------------

Texture2D::~Texture2D()
{
    glDeleteTextures(1, &m_handle);
}

//------------------------------------------------------------------------

void Texture2D::setData(const void* data) noexcept
{
    this->bind();
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_settings->width, m_settings->height, m_settings->format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    this->unbind();
}

//------------------------------------------------------------------------

void Texture2D::setParameteri(uint32_t pname, int32_t param) const noexcept
{
    this->bind();
    glTexParameteri(GL_TEXTURE_2D, pname, param);
    this->unbind();
}

//------------------------------------------------------------------------

std::unique_ptr<Texture2D> Texture2D::makeDefault() noexcept
{
    std::shared_ptr<Settings> settings = std::make_shared<Settings>(1, 1);
    std::unique_ptr<Texture2D> texture = std::make_unique<Texture2D>(settings);
    uint32_t data = 0xffffffff;
    texture->setData(&data);
    return texture;
}

//------------------------------------------------------------------------

void Texture2D::setupTexture() noexcept
{
    glGenTextures(1, &m_handle);
    this->bind();
    glTexStorage2D(GL_TEXTURE_2D, m_settings->levels, m_settings->internalFormat, m_settings->width, m_settings->height);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_settings->wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_settings->wrapT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_settings->minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_settings->magFilter);
    this->unbind();
}

//------------------------------------------------------------------------
