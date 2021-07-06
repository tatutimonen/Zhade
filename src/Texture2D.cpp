#include "Texture2D.hpp"
#include <iostream>

//------------------------------------------------------------------------

Texture2D::Texture2D(const std::shared_ptr<Specification>& spec)
    : m_spec{spec}
{
    setupTexture();
}

//------------------------------------------------------------------------

Texture2D::Texture2D(const std::string& filename)
{
    m_spec = std::make_shared<Specification>();
    int channels = 0;
    unsigned char* imageData = SOIL_load_image(filename.c_str(), &(m_spec->width), &(m_spec->height), &channels, SOIL_LOAD_AUTO);

    if (channels == SOIL_LOAD_RGB) {
        m_spec->internalFormat = GL_RGB8;
        m_spec->format = GL_RGB;
    }
    
    setupTexture();
    setData(imageData);
    SOIL_free_image_data(imageData);
}

//------------------------------------------------------------------------

Texture2D::~Texture2D()
{
    GL_CALL(glDeleteTextures(1, &m_handle));
}

//------------------------------------------------------------------------

void Texture2D::setData(const void* data)
{
    this->bind();
    GL_CALL(glTexSubImage2D(
        GL_TEXTURE_2D, 0, 0, 0, m_spec->width, m_spec->height, m_spec->format, GL_UNSIGNED_BYTE, (const GLvoid*)data)
    );
    GL_CALL(glGenerateMipmap(GL_TEXTURE_2D));
    this->unbind();
}

//------------------------------------------------------------------------

std::unique_ptr<Texture2D> Texture2D::makeDefault()
{
    std::shared_ptr<Specification> spec = std::make_shared<Specification>(1, 1);
    std::unique_ptr<Texture2D> tex = std::make_unique<Texture2D>(spec);
    GLuint data = 0xffffffff;
    tex->setData(&data);
    return tex;
}

//------------------------------------------------------------------------

void Texture2D::setupTexture()
{
    GL_CALL(glGenTextures(1, &m_handle));
    this->bind();
    GL_CALL(glTexStorage2D(GL_TEXTURE_2D, m_spec->levels, m_spec->internalFormat, m_spec->width, m_spec->height));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_spec->wrap_s));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_spec->wrap_t));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_spec->min_filter));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_spec->mag_filter));
    this->unbind();
}

//------------------------------------------------------------------------
