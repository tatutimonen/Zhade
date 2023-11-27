#include "DirectionalLight.hpp"

#include "ResourceManager.hpp"
#include "Texture.hpp"

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

DirectionalLight::DirectionalLight(DirectionalLightDescriptor desc)
    : m_direction{desc.direction},
      m_irradiance{desc.irradiance},
      m_mngr{desc.mngr}
{
    m_framebuffer = m_mngr->createFramebuffer({
        .textureDesc = {
            .dims = desc.shadowMapDims,
            .levels = 1,
            .internalFormat = GL_DEPTH_COMPONENT32,
            .sampler = {
                .wrapS = GL_CLAMP_TO_EDGE,
                .wrapT = GL_CLAMP_TO_EDGE,
                .anisotropy = 1.0f
            }
        },
        .attachment = GL_DEPTH_ATTACHMENT,
        .mngr = m_mngr
    });
}

//------------------------------------------------------------------------

const Framebuffer* DirectionalLight::framebuffer() const noexcept
{
    return m_mngr->get(m_framebuffer);
}

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
