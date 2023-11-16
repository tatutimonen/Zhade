#pragma once

#include "Handle.hpp"
#include "Texture.hpp"
#include "common.hpp"

#include <glm/glm.hpp>
extern "C" {
#include <GL/glew.h>
}

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

class ResourceManager;

struct FramebufferDescriptor
{
    TextureDescriptor texture;
    GLenum attachment;
    ResourceManager* mngr;
    bool managed = true;
};

//------------------------------------------------------------------------

class Framebuffer
{
public:
    Framebuffer() = default;
    explicit Framebuffer(FramebufferDescriptor desc);
    ~Framebuffer();

    Framebuffer(const Framebuffer&) = default;
    Framebuffer& operator=(const Framebuffer&) = default;
    Framebuffer(Framebuffer&&) = delete;
    Framebuffer& operator=(Framebuffer&&) = delete;

    [[nodiscard]] GLuint name() const noexcept { return m_name; }

    void bind() const noexcept { glBindFramebuffer(GL_FRAMEBUFFER, m_name); }
    void freeResources() const noexcept;

private:
    [[nodiscard]] const Texture* texture() const noexcept;

    GLuint m_name = 0;
    Handle<Texture> m_texture{};
    ResourceManager* m_mngr = nullptr;
    bool m_managed = true;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
