#include "Framebuffer.hpp"

#include "ResourceManager.hpp"

#include <fmt/core.h>

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

Framebuffer::Framebuffer(FramebufferDescriptor desc)
    : m_mngr{desc.mngr},
      m_managed{desc.managed}
{
    m_texture = m_mngr->createTexture(desc.textureDesc);

    glCreateFramebuffers(1, &m_name);
    glNamedFramebufferTexture(m_name, desc.attachment, texture()->name(), 0);

    if (desc.attachment == GL_DEPTH_ATTACHMENT) {
        glNamedFramebufferDrawBuffer(m_name, GL_NONE);
    }

    if (glCheckNamedFramebufferStatus(m_name, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        fmt::println("Framebuffer {} is incomplete", m_name);
    }
}

//------------------------------------------------------------------------

Framebuffer::~Framebuffer()
{
    if (not m_managed) freeResources();
}

//------------------------------------------------------------------------

void Framebuffer::freeResources()
{
    m_mngr->destroy(m_texture);
    glDeleteFramebuffers(1, &m_name);
}

//------------------------------------------------------------------------

Texture* Framebuffer::texture()
{
    return m_mngr->get(m_texture);
}

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
