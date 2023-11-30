#include "DirectionalLight.hpp"

#include "ResourceManager.hpp"
#include "Texture.hpp"

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

DirectionalLight::DirectionalLight(DirectionalLightDescriptor desc)
    : m_data{desc.data},
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

    m_uniformBuffer = m_mngr->createBuffer({
        .byteSize = sizeof(DirectionalLightData),
        .usage = BufferUsage::UNIFORM
    });
    uniformBuffer()->bindBase(DIRECTIONAL_LIGHT_BINDING);

    m_pipeline = m_mngr->createPipeline(desc.shadowPassDesc);
}

//------------------------------------------------------------------------

DirectionalLight::~DirectionalLight()
{
    m_mngr->destroy(m_framebuffer);
    m_mngr->destroy(m_uniformBuffer);
    m_mngr->destroy(m_pipeline);
}

//------------------------------------------------------------------------

const Framebuffer* DirectionalLight::framebuffer() const noexcept
{
    return m_mngr->get(m_framebuffer);
}

//------------------------------------------------------------------------

const Buffer* DirectionalLight::uniformBuffer() const noexcept
{
    return m_mngr->get(m_uniformBuffer);
}

//------------------------------------------------------------------------

const Pipeline* DirectionalLight::pipeline() const noexcept
{
    return m_mngr->get(m_pipeline);
}

//------------------------------------------------------------------------

void DirectionalLight::prepareForRendering() const noexcept
{
    framebuffer()->bind();
    pipeline()->bind();
    //glEnable(GL_POLYGON_OFFSET_FILL);
    //glPolygonOffset(2.0f, 4.0f);
}

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
