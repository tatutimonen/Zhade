#include "DirectionalLight.hpp"

#include "ResourceManager.hpp"
#include "Texture.hpp"

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

DirectionalLight::DirectionalLight(DirectionalLightDescriptor desc)
    : m_mngr{desc.mngr},
      m_props{desc.props},
      m_shadowMapDims{desc.shadowMapDims}
{
    setupMatrices(desc);
    setupFramebuffer();
    setupBuffers();
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

void DirectionalLight::prepareForRendering(const Handle<Buffer>& viewProjUniformBuffer)
{
    glViewport(0, 0, m_shadowMapDims.x, m_shadowMapDims.y);
    framebuffer()->bind();
    pipeline()->bind();
    buffer(viewProjUniformBuffer)->setData(&m_matrices);
}

//------------------------------------------------------------------------

Framebuffer* DirectionalLight::framebuffer()
{
    return m_mngr->get(m_framebuffer);
}

//------------------------------------------------------------------------

Buffer* DirectionalLight::buffer(const Handle<Buffer>& handle)
{
    return m_mngr->get(handle);
}

//------------------------------------------------------------------------

Pipeline* DirectionalLight::pipeline()
{
    return m_mngr->get(m_pipeline);
}

//------------------------------------------------------------------------

void DirectionalLight::setupMatrices(const DirectionalLightDescriptor& desc)
{
    auto makeOrtho = [](float extent, float near = 10.0f, float far = 10'000.0f)
    {
        return glm::ortho(
            -extent, extent,
            extent, -extent,
            near, far
        );
    };

    m_matrices = {
        .viewMatT = glm::transpose(glm::lookAt(
            desc.position,
            desc.position + desc.props.direction,
            util::makeUnitVec3y()
        )),
        .projMat = makeOrtho(1000.0f)
    };
}

//------------------------------------------------------------------------

void DirectionalLight::setupFramebuffer()
{
    m_framebuffer = m_mngr->createFramebuffer({
        .textureDesc = {
            .dims = m_shadowMapDims,
            .levels = 1,
            .internalFormat = GL_DEPTH_COMPONENT32F,
            .sampler = {
                .wrapS = GL_CLAMP_TO_EDGE,
                .wrapT = GL_CLAMP_TO_EDGE,
                .magFilter = GL_LINEAR,
                .minFilter = GL_LINEAR,
                .anisotropy = 1.0f
            }
        },
        .attachment = GL_DEPTH_ATTACHMENT,
        .mngr = m_mngr
    });
}

//------------------------------------------------------------------------

void DirectionalLight::setupBuffers()
{
    m_propsBuffer = m_mngr->createBuffer({
        .byteSize = sizeof(DirectionalLightProperties),
        .usage = BufferUsage::STORAGE,
        .indexedBindings = {
            {.target = BufferUsage::STORAGE, .index = DIRECTIONAL_LIGHT_PROPS_BINDING}
        }
    });
    buffer(m_propsBuffer)->setData(&m_props);

    m_depthTextureBuffer = m_mngr->createBuffer({
        .byteSize = sizeof(GLuint64),
        .usage = BufferUsage::UNIFORM,
        .indexedBindings = {
            {.target = BufferUsage::UNIFORM, .index = DIRECTIONAL_LIGHT_DEPTH_TEXTURE_BINDING}
        }
    });
    const GLuint64 depthTextureHandle = framebuffer()->texture()->handle();
    buffer(m_depthTextureBuffer)->setData(&depthTextureHandle);

    m_shadowMatrixBuffer = m_mngr->createBuffer({
        .byteSize = sizeof(glm::mat4),
        .usage = BufferUsage::UNIFORM,
        .indexedBindings = {
            {.target = BufferUsage::UNIFORM, .index = DIRECTIONAL_LIGHT_SHADOW_MATRIX_BINDING}
        }
    });
    const glm::mat4 shadowMatrix = (
        glm::mat4{
            0.5f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.5f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.5f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f
        }
        * m_matrices.projMat
        * glm::mat4(glm::transpose(m_matrices.viewMatT))
    );
    buffer(m_shadowMatrixBuffer)->setData(&shadowMatrix);
}

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
