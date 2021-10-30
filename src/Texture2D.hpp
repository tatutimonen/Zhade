#pragma once

#include "Texture.hpp"

//------------------------------------------------------------------------

class Texture2D final : Texture {
public:
    struct Settings : Texture::Settings {
        GLsizei height = 1;

        GLenum wrapT = GL_CLAMP_TO_EDGE;

        Settings() = default;
        Settings(GLsizei width, GLsizei height)
            : Texture::Settings(width),
              height{height}
        {}
    };

    Texture2D(const std::string& filename);
    Texture2D(const std::shared_ptr<Settings>& settings);
    ~Texture2D();

    virtual void setData(const void* data) override;

    virtual inline void bind() const noexcept override   { CHECK_GL_ERROR(glBindTexture(GL_TEXTURE_2D, m_handle)); }
    virtual inline void unbind() const noexcept override { CHECK_GL_ERROR(glBindTexture(GL_TEXTURE_2D, 0)); }

    static std::unique_ptr<Texture2D> makeDefault();

private:
    void setupTexture();

    GLuint m_handle;
    std::shared_ptr<Settings> m_settings;
};

//------------------------------------------------------------------------
