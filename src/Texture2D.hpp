#pragma once

#include "ITexture.hpp"

//------------------------------------------------------------------------

class Texture2D final : ITexture {
public:
    struct Specification : ITexture::Specification {
        GLsizei height = 1;

        GLenum wrap_t = GL_CLAMP_TO_EDGE;

        Specification() = default;
        Specification(GLsizei width, GLsizei height)
            : ITexture::Specification(width),
              height{height}
        {}
    };

    Texture2D(const std::string& filename);
    Texture2D(const std::shared_ptr<Specification>& spec);
    ~Texture2D();

    virtual void setData(const void* data) override;

    virtual inline void bind() const noexcept override   { GL_CALL(glBindTexture(GL_TEXTURE_2D, m_handle)); }
    virtual inline void unbind() const noexcept override { GL_CALL(glBindTexture(GL_TEXTURE_2D, 0)); }

    static std::unique_ptr<Texture2D> makeDefault();

private:
    void setupTexture();

    GLuint m_handle;
    std::shared_ptr<Specification> m_spec;
};

//------------------------------------------------------------------------
