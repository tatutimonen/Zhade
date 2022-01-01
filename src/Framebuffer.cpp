#include "Framebuffer.hpp"

//------------------------------------------------------------------------

Framebuffer::Framebuffer(const glm::uvec2& dimensions)
    : m_dimensions{dimensions}
{
    glGenFramebuffers(1, &m_handle);
}

//------------------------------------------------------------------------

Framebuffer::~Framebuffer()
{
    glDeleteFramebuffers(1, &m_handle);
}

//------------------------------------------------------------------------

void Framebuffer::bind(uint32_t target = GL_FRAMEBUFFER) const noexcept
{
    glBindFramebuffer(target, m_handle);
    glViewport(0, 0, m_dimensions.x, m_dimensions.y);
}

//------------------------------------------------------------------------

void Framebuffer::unbind(uint32_t target = GL_FRAMEBUFFER) const noexcept
{
    glBindFramebuffer(target, 0);
    glViewport(0, 0, App::window_width, App::window_height);
}

//------------------------------------------------------------------------

void Framebuffer::attachTexture2D(const std::shared_ptr<Texture2D>& texture, uint32_t attachment) noexcept
{
    assert(texture->getSettings().width == m_dimensions.x && texture->getSettings().height == m_dimensions.y);
    if (m_textureAttachmentMemo.find(texture->m_handle) != m_textureAttachmentMemo.end())
        return;

    m_textureAttachmentMemo.insert(texture->m_handle);
    m_textureAttachments.push_back(texture);
    glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, texture->m_handle, 0);
} 

//------------------------------------------------------------------------
