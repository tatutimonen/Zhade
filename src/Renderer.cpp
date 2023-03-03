#include "Renderer.hpp"

#include <cassert>
#include <numeric>
#include <ranges>
#include <vector>

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

Renderer::Renderer(ResourceManager* mngr, const Specification& spec)
    : m_mngr{mngr},
      m_programs{spec.programs.begin(), spec.programs.end()}
{
    glCreateVertexArrays(1, &m_vao);

    glEnableVertexArrayAttrib(m_vao, 0);
    glEnableVertexArrayAttrib(m_vao, 1);
    glEnableVertexArrayAttrib(m_vao, 2);

    glVertexArrayAttribFormat(m_vao, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, pos));
    glVertexArrayAttribFormat(m_vao, 1, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, nrm));
    glVertexArrayAttribFormat(m_vao, 2, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, tex));

    for (GLuint idx : std::views::iota(0u, spec.vertexBuffers.size()))
    {
        const GLuint vboHandle = m_mngr->get(spec.vertexBuffers[idx])->getGLHandle();
        glVertexArrayVertexBuffer(m_vao, idx, vboHandle, 0, sizeof(Vertex));
        glVertexArrayAttribBinding(m_vao, 0, idx);
        glVertexArrayAttribBinding(m_vao, 1, idx);
        glVertexArrayAttribBinding(m_vao, 2, idx);
    }

    glVertexArrayElementBuffer(m_vao, m_mngr->get(spec.indexBuffer)->getGLHandle());

    m_drawIndirectBuffer = m_mngr->createBuffer(GL_DRAW_INDIRECT_BUFFER, 1 << 10);
    m_transformsBuffer = m_mngr->createBuffer(GL_SHADER_STORAGE_BUFFER, 1 << 16);
}

//------------------------------------------------------------------------

Renderer::~Renderer()
{
    glDeleteVertexArrays(1, &m_vao);
}

//------------------------------------------------------------------------

void Renderer::submit(const Job& job) const noexcept
{
    assert(job.instanceCount == job.transformations.size());
    m_jobs.push_back(job);
}

//------------------------------------------------------------------------

void Renderer::render() const noexcept
{
    if (const auto drawIndirectBuffer = m_mngr->get(m_drawIndirectBuffer);
        !drawIndirectBuffer->fits<MultiDrawElementsIndirectCommand>(m_jobs.size()))
    {
        m_mngr->destroy(m_drawIndirectBuffer);
        m_drawIndirectBuffer = m_mngr->createBuffer(
            GL_DRAW_INDIRECT_BUFFER, static_cast<GLsizei>(sizeof(MultiDrawElementsIndirectCommand) * m_jobs.size()));
    }
    else
    {
        drawIndirectBuffer->zero();
    }

    const auto numTransforms = std::accumulate(m_jobs.begin(), m_jobs.end(), 0,
        [](int total, const Job& job)
        {
            return total + job.instanceCount;
        });

    if (const auto transformBuffer = m_mngr->get(m_transformsBuffer);
        !transformBuffer->fits<glm::mat3x4>(numTransforms))
    {
        m_mngr->destroy(m_transformsBuffer);
        m_transformsBuffer = m_mngr->createBuffer(
            GL_SHADER_STORAGE_BUFFER, static_cast<GLsizei>(sizeof(glm::mat3x4) * numTransforms));
    }
    else
    {
        transformBuffer->zero();
    }

    std::vector<MultiDrawElementsIndirectCommand> cmds;
    cmds.reserve(m_jobs.size());
    std::vector<glm::mat3x4> transforms;
    transforms.reserve(m_jobs.size());

    struct LoopState
    {
        GLuint firstIndex;
        GLuint baseVertex;
        GLuint baseInstance;
    };
    auto loopState = LoopState{};

    for (const auto& job : m_jobs)
    {
        const Model* model = m_mngr->get(job.model);

        cmds.push_back({
            .vertexCount = static_cast<GLuint>(model->getNumIndices()),
            .instanceCount = job.instanceCount,
            .firstIndex = loopState.firstIndex,
            .baseVertex = loopState.baseVertex,
            .baseInstance = loopState.baseInstance
        });

        for (const auto& transform : job.transformations)
            transforms.push_back(transform);

        loopState.firstIndex += model->getNumIndices();
        loopState.baseVertex += model->getNumVertices();
        loopState.baseInstance += job.instanceCount;
    }

    m_mngr->get(m_drawIndirectBuffer)->setData<MultiDrawElementsIndirectCommand>(cmds.data(), cmds.size(), 0);
    m_mngr->get(m_transformsBuffer)->setData<glm::mat3x4>(transforms.data(), transforms.size(), 0);

    m_mngr->get(m_drawIndirectBuffer)->bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, nullptr, cmds.size(), 0);
}

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
