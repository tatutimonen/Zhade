#include "DirectionalLight.hpp"

#include "ResourceManager.hpp"
#include "Texture.hpp"

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

DirectionalLight::DirectionalLight(DirectionalLightDescriptor desc)
    : m_props{desc.props},
      m_shadowMapDims{desc.shadowMapDims},
      m_mngr{desc.mngr}
{
    m_matrices = {
        .viewMatT = glm::transpose(glm::lookAt(desc.position, desc.position + desc.target, util::makeUnitVec3y())),
        .projMat = glm::ortho(
            0.0f, static_cast<float>(m_shadowMapDims.x),
            0.0f, static_cast<float>(m_shadowMapDims.y), 
            10.0f, 10'000.0f
        )
    };

    m_framebuffer = m_mngr->createFramebuffer({
        .textureDesc = {
            .dims = m_shadowMapDims,
            .levels = 1,
            .internalFormat = GL_DEPTH_COMPONENT32F,
            .sampler = {
                .wrapS = GL_CLAMP_TO_EDGE,
                .wrapT = GL_CLAMP_TO_EDGE,
                .anisotropy = 1.0f
            }
        },
        .attachment = GL_DEPTH_ATTACHMENT,
        .mngr = m_mngr
    });

    m_propsBuffer = m_mngr->createBuffer({
        .byteSize = sizeof(DirectionalLightProperties),
        .usage = BufferUsage::STORAGE
    });
    buffer(m_propsBuffer)->bindBase(DIRECTIONAL_LIGHT_PROPS_BINDING);
    buffer(m_propsBuffer)->setData(&m_props);

    m_depthTextureBuffer = m_mngr->createBuffer({
        .byteSize = sizeof(GLuint64),
        .usage = BufferUsage::UNIFORM
    });
    buffer(m_depthTextureBuffer)->bindBase(DIRECTIONAL_LIGHT_DEPTH_TEXTURE_BINDING);

    m_pipeline = m_mngr->createPipeline(desc.shadowPassDesc);
}

//------------------------------------------------------------------------

DirectionalLight::~DirectionalLight()
{
    m_mngr->destroy(m_framebuffer);
    m_mngr->destroy(m_propsBuffer);
    m_mngr->destroy(m_depthTextureBuffer);
    m_mngr->destroy(m_pipeline);
}

//------------------------------------------------------------------------

const Framebuffer* DirectionalLight::framebuffer() const noexcept
{
    return m_mngr->get(m_framebuffer);
}

//------------------------------------------------------------------------

const Buffer* DirectionalLight::buffer(const Handle<Buffer>& handle) const noexcept
{
    return m_mngr->get(handle);
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
    glViewport(0, 0, m_shadowMapDims.x, m_shadowMapDims.y);
    //glEnable(GL_POLYGON_OFFSET_FILL);
    //glPolygonOffset(2.0f, 4.0f);
}

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
