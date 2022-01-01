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

void Texture2D::setData(const void* data)
{
    this->bind();
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_settings->width, m_settings->height,
        m_settings->format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    this->unbind();
}

//------------------------------------------------------------------------

std::unique_ptr<Texture2D> Texture2D::makeDefault()
{
    std::shared_ptr<Settings> settings = std::make_shared<Settings>(1, 1);
    std::unique_ptr<Texture2D> tex = std::make_unique<Texture2D>(settings);
    uint32_t data = 0xffffffff;
    tex->setData(&data);
    return tex;
}

//------------------------------------------------------------------------

void Texture2D::setupTexture()
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
