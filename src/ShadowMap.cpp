#include "ShadowMap.hpp"

//------------------------------------------------------------------------

ShadowMap::ShadowMap(const glm::uvec2& dimensions)
    : m_dimensions{dimensions},
      m_framebuffer{Framebuffer(m_dimensions)}
{
    auto depthTextureSettings = std::make_shared<Texture2D::Settings>(m_dimensions.x, m_dimensions.y);
    depthTextureSettings->internalFormat = GL_DEPTH_COMPONENT32;
    depthTextureSettings->format = GL_DEPTH_COMPONENT;
    depthTextureSettings->type = GL_FLOAT;
    
    m_depthTexture = std::make_shared<Texture2D>(depthTextureSettings);
    m_depthTexture->setParameteri(GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    m_depthTexture->setParameteri(GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

    m_framebuffer.attachTexture2D(m_depthTexture, GL_DEPTH_ATTACHMENT);
}

//------------------------------------------------------------------------
