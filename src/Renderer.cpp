#include "Renderer.hpp"

#include "common.hpp"

#include <array>
#include <cstring>
#include <memory_resource>
#include <numeric>
#include <ranges>

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

Renderer::Renderer(RendererDescriptor desc)
    : m_mngr{desc.mngr},
      m_scene{desc.sceneDesc}
{
    setupVAO();
    setupBuffers(desc);
    setupCamera(desc.cameraDesc);
    setupPipeline(desc.mainPassDesc);
}

//------------------------------------------------------------------------

Renderer::~Renderer()
{
    glDeleteVertexArrays(1, &m_vao);
    m_mngr->destroy(m_commandBuffer);
    m_mngr->destroy(m_drawMetadataBuffer);
    m_mngr->destroy(m_atomicDrawCounterBuffer);
    m_mngr->destroy(m_viewProjUniformBuffer);
    m_mngr->destroy(m_pipeline);
}

//------------------------------------------------------------------------

void Renderer::render()
{
    m_scene.m_sunLight.prepareForRendering(m_viewProjUniformBuffer);
    populateBuffers();
    glClear(GL_DEPTH_BUFFER_BIT);
    glMultiDrawElementsIndirectCount(GL_TRIANGLES, GL_UNSIGNED_INT, nullptr, 0, MAX_DRAWS, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, App::s_windowWidth, App::s_windowHeight);
    pipeline()->bind();
    buffer(m_viewProjUniformBuffer)->setData(&m_camera.m_matrices);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMultiDrawElementsIndirectCount(GL_TRIANGLES, GL_UNSIGNED_INT, nullptr, 0, MAX_DRAWS, 0);

    clearDrawCounter();
}

//------------------------------------------------------------------------

void Renderer::setupVAO()
{
    glCreateVertexArrays(1, &m_vao);

    glVertexArrayVertexBuffer(m_vao, 0, buffer(m_scene.m_vertexBuffer)->name(), 0, sizeof(Vertex));
    glVertexArrayElementBuffer(m_vao, buffer(m_scene.m_indexBuffer)->name());

    glEnableVertexArrayAttrib(m_vao, 0);
    glEnableVertexArrayAttrib(m_vao, 1);
    glEnableVertexArrayAttrib(m_vao, 2);

    glVertexArrayAttribFormat(m_vao, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, pos));
    glVertexArrayAttribFormat(m_vao, 1, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, nrm));
    glVertexArrayAttribFormat(m_vao, 2, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, uv));

    glVertexArrayAttribBinding(m_vao, 0, 0);
    glVertexArrayAttribBinding(m_vao, 1, 0);
    glVertexArrayAttribBinding(m_vao, 2, 0);

    glBindVertexArray(m_vao);
} 

//------------------------------------------------------------------------

void Renderer::setupBuffers(const RendererDescriptor& desc)
{
    m_commandBuffer = m_mngr->createBuffer({
        .byteSize = MAX_DRAWS * sizeof(DrawElementsIndirectCommand),
        .usage = BufferUsage::INDIRECT,
        .bindings = { BufferUsage::INDIRECT },
        .indexedBindings = {
            {.target = BufferUsage::STORAGE, .index = INDIRECT_BINDING}
        }
    });

    m_drawMetadataBuffer = m_mngr->createBuffer({
        .byteSize = MAX_DRAWS * sizeof(DrawMetadata),
        .usage = BufferUsage::STORAGE,
        .indexedBindings = {
            {.target = BufferUsage::STORAGE, .index = DRAW_METADATA_BINDING}
        }
    });

    m_atomicDrawCounterBuffer = m_mngr->createBuffer({
        .byteSize = sizeof(GLuint),
        .usage = BufferUsage::ATOMIC_COUNTER,
        .bindings = { BufferUsage::PARAMETER },
        .indexedBindings = {
            {.target = BufferUsage::ATOMIC_COUNTER, .index = ATOMIC_COUNTER_BINDING}
        }
    });

    m_viewProjUniformBuffer = m_mngr->createBuffer({
        .byteSize = sizeof(ViewProjMatrices),
        .usage = BufferUsage::UNIFORM,
        .indexedBindings = {
            {.target = BufferUsage::UNIFORM, .index = VIEW_PROJ_BINDING}
        }
    });
}

//------------------------------------------------------------------------

void Renderer::setupCamera(CameraDescriptor cameraDesc)
{
    cameraDesc.uniformBuffer = m_viewProjUniformBuffer;
    m_camera = Camera(cameraDesc);
}

//------------------------------------------------------------------------

void Renderer::setupPipeline(PipelineDescriptor mainPassDesc)
{
    mainPassDesc.managed = true;
    m_pipeline = m_mngr->createPipeline(mainPassDesc);
    pipeline()->bind();
}

//------------------------------------------------------------------------

void Renderer::populateBuffers()
{
    const size_t numMeshes = buffer(m_scene.m_meshBuffer)->size<Mesh>();
    glDispatchCompute(util::divup(numMeshes, WORK_GROUP_LOCAL_SIZE_X), 1, 1);
}

//------------------------------------------------------------------------

void Renderer::clearDrawCounter()
{
    buffer(m_atomicDrawCounterBuffer)->invalidate();
    static constexpr GLuint zero = 0;
    glClearNamedBufferData(buffer(m_atomicDrawCounterBuffer)->name(), GL_R32UI, GL_RED, GL_UNSIGNED_INT, &zero);
}

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
