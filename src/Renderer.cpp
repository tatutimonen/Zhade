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

void Renderer::render() const noexcept
{
    populateBuffers();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMultiDrawElementsIndirectCount(GL_TRIANGLES, GL_UNSIGNED_INT, nullptr, 0, MAX_DRAWS, 0);
    clearDrawCounter();
}

//------------------------------------------------------------------------

void Renderer::setupVAO() noexcept
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

void Renderer::setupBuffers(const RendererDescriptor& desc) noexcept
{
    m_commandBuffer = m_mngr->createBuffer(desc.commandBufferDesc);
    m_drawMetadataBuffer = m_mngr->createBuffer(desc.drawMetadataBuffer);
    m_atomicDrawCounterBuffer = m_mngr->createBuffer({.byteSize = sizeof(GLuint), .usage = BufferUsage::ATOMIC_COUNTER});
    m_viewProjUniformBuffer = m_mngr->createBuffer({.byteSize = sizeof(ViewProjMatrices), .usage = BufferUsage::UNIFORM});

    buffer(m_commandBuffer)->bind();
    buffer(m_commandBuffer)->bindBaseAs(INDIRECT_BINDING, BufferUsage::STORAGE);
    buffer(m_drawMetadataBuffer)->bindBase(DRAW_METADATA_BINDING);
    buffer(m_scene.m_meshBuffer)->bindBase(MESH_BINDING);
    buffer(m_atomicDrawCounterBuffer)->bindBase(ATOMIC_COUNTER_BINDING);
    buffer(m_atomicDrawCounterBuffer)->bindAs(BufferUsage::PARAMETER);
    buffer(m_viewProjUniformBuffer)->bindBase(VIEW_PROJ_BINDING);
}

//------------------------------------------------------------------------

void Renderer::setupCamera(CameraDescriptor& cameraDesc) noexcept
{
    cameraDesc.uniformBuffer = m_viewProjUniformBuffer;
    m_camera = Camera(cameraDesc);
}

//------------------------------------------------------------------------

void Renderer::setupPipeline(PipelineDescriptor& mainPassDesc) noexcept
{
    mainPassDesc.managed = true;
    m_pipeline = m_mngr->createPipeline(mainPassDesc);
    pipeline()->bind();
}

//------------------------------------------------------------------------

void Renderer::populateBuffers() const noexcept
{
    const size_t numMeshes = buffer(m_scene.m_meshBuffer)->size<Mesh>();
    glDispatchCompute(util::divup(numMeshes, WORK_GROUP_LOCAL_SIZE_X), 1, 1);
}

//------------------------------------------------------------------------

void Renderer::clearDrawCounter() const noexcept
{
    buffer(m_atomicDrawCounterBuffer)->invalidate();
    static constexpr GLuint zero = 0;
    glClearNamedBufferData(buffer(m_atomicDrawCounterBuffer)->name(), GL_R32UI, GL_RED, GL_UNSIGNED_INT, &zero);
}

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
