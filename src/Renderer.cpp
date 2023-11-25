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
      m_scene{desc.scene},
      m_mainPipeline{Pipeline(desc.mainPipelineDesc)}
{
    glCreateVertexArrays(1, &m_vao);

    glVertexArrayVertexBuffer(m_vao, 0, m_scene->vertexBuffer()->name(), 0, sizeof(Vertex));
    glVertexArrayElementBuffer(m_vao, m_scene->indexBuffer()->name());

    glEnableVertexArrayAttrib(m_vao, 0);
    glEnableVertexArrayAttrib(m_vao, 1);
    glEnableVertexArrayAttrib(m_vao, 2);

    glVertexArrayAttribFormat(m_vao, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, pos));
    glVertexArrayAttribFormat(m_vao, 1, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, nrm));
    glVertexArrayAttribFormat(m_vao, 2, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, uv));

    glVertexArrayAttribBinding(m_vao, 0, 0);
    glVertexArrayAttribBinding(m_vao, 1, 0);
    glVertexArrayAttribBinding(m_vao, 2, 0);

    m_commandBuffer = m_mngr->createBuffer(desc.commandBufferDesc);
    m_drawMetadataBuffer = m_mngr->createBuffer(desc.drawMetadataBuffer);
    m_atomicDrawCounterBuffer = m_mngr->createBuffer({.byteSize = sizeof(GLuint), .usage = BufferUsage::ATOMIC_COUNTER});

    glBindVertexArray(m_vao);
    m_mainPipeline.bind();
    commandBuffer()->bind();
    commandBuffer()->bindBaseAs(INDIRECT_BINDING, BufferUsage::STORAGE);
    drawMetadataBuffer()->bindBase(DRAW_METADATA_BINDING);
    meshBuffer()->bindBase(MESH_BINDING);
    atomicDrawCounterBuffer()->bindBase(ATOMIC_COUNTER_BINDING);
    atomicDrawCounterBuffer()->bindAs(BufferUsage::PARAMETER);
}

//------------------------------------------------------------------------

Renderer::~Renderer()
{
    glDeleteVertexArrays(1, &m_vao);
    m_mngr->destroy(m_commandBuffer);
    m_mngr->destroy(m_drawMetadataBuffer);
    m_mngr->destroy(m_atomicDrawCounterBuffer);
}

//------------------------------------------------------------------------

void Renderer::render() const noexcept
{
    populateBuffers();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glMultiDrawElementsIndirectCount(GL_TRIANGLES, GL_UNSIGNED_INT, nullptr, 0, MAX_DRAWS, 0);
    clearDrawCounter();
}

//------------------------------------------------------------------------

void Renderer::populateBuffers() const noexcept
{
    glDispatchCompute(util::divup(meshBuffer()->size<Mesh>(), WORK_GROUP_LOCAL_SIZE_X), 1, 1);
}

//------------------------------------------------------------------------

void Renderer::clearDrawCounter() const noexcept
{
    atomicDrawCounterBuffer()->invalidate();
    static constexpr GLuint zero = 0;
    glClearNamedBufferData(atomicDrawCounterBuffer()->name(), GL_R32UI, GL_RED, GL_UNSIGNED_INT, &zero);
}

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
