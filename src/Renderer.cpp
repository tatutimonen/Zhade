#include "Renderer.hpp"

#include <iostream>
#include <ranges>
#include <vector>

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

Renderer::Renderer(ResourceManager* mngr, const Specification& spec)
    : m_mngr{mngr},
      m_program{std::move(spec.program)}
{
    glCreateVertexArrays(1, &m_vao);

    glEnableVertexArrayAttrib(m_vao, 0);
    glEnableVertexArrayAttrib(m_vao, 1);
    glEnableVertexArrayAttrib(m_vao, 2);

    glVertexArrayAttribFormat(m_vao, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, pos));
    glVertexArrayAttribFormat(m_vao, 1, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, nrm));
    glVertexArrayAttribFormat(m_vao, 2, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, tex));

    const GLuint vboHandle = m_mngr->get(spec.vertexBuffer)->getGLHandle();
    glVertexArrayVertexBuffer(m_vao, 0, vboHandle, 0, sizeof(Vertex));
    glVertexArrayAttribBinding(m_vao, 0, 0);
    glVertexArrayAttribBinding(m_vao, 1, 0);
    glVertexArrayAttribBinding(m_vao, 2, 0);

    glVertexArrayElementBuffer(m_vao, m_mngr->get(spec.indexBuffer)->getGLHandle());

    m_drawIndirectBuffer = m_mngr->createBuffer(GL_DRAW_INDIRECT_BUFFER, 1 << 10);
    m_transformsBuffer = m_mngr->createBuffer(GL_SHADER_STORAGE_BUFFER, 1 << 16);

    glBindVertexArray(m_vao);
    m_mngr->get(m_drawIndirectBuffer)->bind();
    m_program->use();
}

//------------------------------------------------------------------------

Renderer::~Renderer()
{
    glDeleteVertexArrays(1, &m_vao);
}

//------------------------------------------------------------------------

void Renderer::submit(const Task& task) const noexcept
{
    if (task.instanceCount != task.transformations.size()) [[unlikely]]
    {
        std::cerr << "instanceCount does not match transformations.size()\n";
        return;
    }
    m_tasks.push_back(task);
}

//------------------------------------------------------------------------

void Renderer::render() const noexcept
{
    // TODO: Ensure these buffers are large enough.
    const auto drawIndirectBuffer = m_mngr->get(m_drawIndirectBuffer);
    const auto transformsBuffer = m_mngr->get(m_transformsBuffer);
    drawIndirectBuffer->zero();
    transformsBuffer->zero();

    std::vector<MultiDrawElementsIndirectCommand> cmds;
    cmds.reserve(m_tasks.size());
    std::vector<glm::mat3x4> transforms;
    transforms.reserve(m_tasks.size());

    GLuint firstIndex = 0;
    GLuint baseVertex = 0;
    GLuint baseInstance = 0;
    for (const auto& task : m_tasks)
    {
        const Model* model = m_mngr->get(task.model);

        cmds.push_back({
            .vertexCount = static_cast<GLuint>(model->getNumIndices()),
            .instanceCount = task.instanceCount,
            .firstIndex = firstIndex,
            .baseVertex = baseVertex,
            .baseInstance = baseInstance
        });

        for (const auto& transform : task.transformations)
            transforms.push_back(transform);

        firstIndex += model->getNumIndices();
        baseVertex += model->getNumVertices();
        baseInstance += task.instanceCount;
    }

    drawIndirectBuffer->setData<MultiDrawElementsIndirectCommand>(cmds.data(), cmds.size(), 0);
    transformsBuffer->setData<glm::mat3x4>(transforms.data(), transforms.size(), 0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, nullptr, cmds.size(), 0);
}

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
