#include "Framebuffer.hpp"

#include "ResourceManager.hpp"

#include <print>

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

Framebuffer::Framebuffer(FramebufferDescriptor desc)
    : m_mngr{desc.mngr},
      m_texture{desc.mngr->createTexture(desc.texture)},
      m_managed{desc.managed}
{
    glCreateFramebuffers(1, &m_name);
    glNamedFramebufferTexture(m_name, desc.attachment, texture()->getName(), 0);

    if (glCheckNamedFramebufferStatus(m_name, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) [[unlikely]]
    {
        std::println("Framebuffer with ID {} is incomplete", m_name);
    }
}

//------------------------------------------------------------------------

Framebuffer::~Framebuffer()
{
    if (m_managed) [[likely]] return;
    freeResources();
}

//------------------------------------------------------------------------

void Framebuffer::freeResources() const noexcept
{
    m_mngr->destroy(m_texture);
    glDeleteFramebuffers(1, &m_name);
}

//------------------------------------------------------------------------

const Texture* Framebuffer::texture() const noexcept
{
    return m_mngr->get(m_texture);
}

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
