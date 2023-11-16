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
    m_transformBuffer = m_mngr->createBuffer(desc.transformBufferDesc);
    m_textureBuffer = m_mngr->createBuffer(desc.textureBufferDesc);
    m_atomicCounterBuffer = m_mngr->createBuffer({.byteSize = sizeof(GLuint), .usage = BufferUsage::ATOMIC_COUNTER});
    m_parameterBuffer = m_mngr->createBuffer({.byteSize = sizeof(GLuint), .usage = BufferUsage::PARAMETER});

    glBindVertexArray(m_vao);
    m_mainPipeline.bind();
    commandBuffer()->bind();
    transformBuffer()->bindBase(MODEL_BINDING);
    textureBuffer()->bindBase(TEXTURE_BINDING);
    atomicCounterBuffer()->bindBase(ATOMIC_COUNTER_BINDING);
    parameterBuffer()->bind();
}

//------------------------------------------------------------------------

Renderer::~Renderer()
{
    glDeleteVertexArrays(1, &m_vao);
    m_mngr->destroy(m_commandBuffer);
    m_mngr->destroy(m_transformBuffer);
    m_mngr->destroy(m_textureBuffer);
    m_mngr->destroy(m_atomicCounterBuffer);
    m_mngr->destroy(m_parameterBuffer);
}

//------------------------------------------------------------------------

void Renderer::render() const noexcept
{
    populateBuffers();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glMultiDrawElementsIndirectCount(GL_TRIANGLES, GL_UNSIGNED_INT, nullptr, 0, MAX_DRAWS, 0);
}

//------------------------------------------------------------------------

void Renderer::populateBuffers() const noexcept
{
    invalidateBuffers();
    glDispatchCompute(WORK_GROUP_LOCAL_SIZE_X, WORK_GROUP_LOCAL_SIZE_Y, WORK_GROUP_LOCAL_SIZE_Z);
    glCopyNamedBufferSubData(atomicCounterBuffer()->name(), parameterBuffer()->name(), 0, 0, sizeof(GLuint));
}

//------------------------------------------------------------------------

void Renderer::invalidateBuffers() const noexcept
{
    commandBuffer()->invalidate();
    transformBuffer()->invalidate();
    textureBuffer()->invalidate();
    m_scene->meshBuffer()->invalidate();
}

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
