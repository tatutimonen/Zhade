#include "Renderer.hpp"
#include "constants.hpp"

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

Renderer::Renderer(ResourceManager* mngr, const Specification& spec)
    : m_mngr{mngr},
      m_vertexBuffer{spec.vertexBuffer},
      m_indexBuffer{spec.indexBuffer},
      m_program{spec.program}
{
    glCreateVertexArrays(1, &m_vao);

    glVertexArrayVertexBuffer(m_vao, 0, m_mngr->get(m_vertexBuffer)->getGLHandle(), 0, sizeof(Vertex));
    glVertexArrayElementBuffer(m_vao, m_mngr->get(m_indexBuffer)->getGLHandle());

    glEnableVertexArrayAttrib(m_vao, 0);
    glEnableVertexArrayAttrib(m_vao, 1);
    glEnableVertexArrayAttrib(m_vao, 2);

    glVertexArrayAttribFormat(m_vao, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, pos));
    glVertexArrayAttribFormat(m_vao, 1, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, nrm));
    glVertexArrayAttribFormat(m_vao, 2, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, tex));

    glVertexArrayAttribBinding(m_vao, 0, 0);
    glVertexArrayAttribBinding(m_vao, 1, 0);
    glVertexArrayAttribBinding(m_vao, 2, 0);

    m_drawIndirectBuffer = m_mngr->createBuffer(GL_DRAW_INDIRECT_BUFFER, 1 << 10);
    m_transformsBuffer = m_mngr->createBuffer(GL_SHADER_STORAGE_BUFFER, 1 << 16);
    auto dibo = m_mngr->get(m_drawIndirectBuffer);
    auto tbo = m_mngr->get(m_transformsBuffer);
    cmdData = dibo->mapRange<MultiDrawElementsIndirectCommand>(0, dibo->getWholeSizeBytes());
    transformsData = tbo->mapRange<glm::mat3x4>(0, tbo->getWholeSizeBytes());

    glBindVertexArray(m_vao);
    dibo->bind();
    tbo->bindBase(constants::MODEL_BINDING);
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
    const auto vbo = m_mngr->get(m_vertexBuffer);
    const auto ebo = m_mngr->get(m_indexBuffer);
    vbo->invalidate();
    ebo->invalidate();

    // TODO: Ensure these buffers are large enough.
    const auto drawIndirectBuffer = m_mngr->get(m_drawIndirectBuffer);
    const auto transformsBuffer = m_mngr->get(m_transformsBuffer);
    drawIndirectBuffer->invalidate();
    transformsBuffer->invalidate();

    GLuint firstIndex = 0;
    GLuint baseVertex = 0;
    GLuint baseInstance = 0;
    GLuint cmdIdx = 0;
    GLuint transformIdx = 0;

    for (const auto& task : m_tasks)
    {
        const Model* model = m_mngr->get(task.model);

        vbo->pushData(model->getVertices().data(), model->getNumVertices());
        ebo->pushData(model->getIndices().data(), model->getNumIndices());

        MultiDrawElementsIndirectCommand cmd = {
            .vertexCount = static_cast<GLuint>(model->getNumIndices()),
            .instanceCount = task.instanceCount,
            .firstIndex = firstIndex,
            .baseVertex = baseVertex,
            .baseInstance = baseInstance
        };
        cmdData[cmdIdx++] = cmd;

        for (const auto& transform : task.transformations)
            transformsData[transformIdx++] = transform;

        firstIndex += model->getNumIndices();
        baseVertex += model->getNumVertices();
        baseInstance += task.instanceCount;
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, nullptr, m_tasks.size(), 0);
}

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
