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

    glVertexArrayVertexBuffer(m_vao, 0, m_mngr->get(m_vertexBuffer)->getName(), 0, sizeof(Vertex));
    glVertexArrayElementBuffer(m_vao, m_mngr->get(m_indexBuffer)->getName());

    glEnableVertexArrayAttrib(m_vao, 0);
    glEnableVertexArrayAttrib(m_vao, 1);
    glEnableVertexArrayAttrib(m_vao, 2);

    glVertexArrayAttribFormat(m_vao, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, pos));
    glVertexArrayAttribFormat(m_vao, 1, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, nrm));
    glVertexArrayAttribFormat(m_vao, 2, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, uv));

    glVertexArrayAttribBinding(m_vao, 0, 0);
    glVertexArrayAttribBinding(m_vao, 1, 0);
    glVertexArrayAttribBinding(m_vao, 2, 0);

    m_drawIndirectBuffer = m_mngr->createBuffer(GL_DRAW_INDIRECT_BUFFER, 1 << 10);
    m_transformsBuffer = m_mngr->createBuffer(GL_SHADER_STORAGE_BUFFER, 1 << 16);
    m_textureBuffer = m_mngr->createBuffer(GL_SHADER_STORAGE_BUFFER, 64);
    auto dibo = m_mngr->get(m_drawIndirectBuffer);
    auto tbo = m_mngr->get(m_transformsBuffer);
    auto tebo = m_mngr->get(m_textureBuffer);

    glBindVertexArray(m_vao);
    dibo->bind();
    tbo->bindBase(constants::MODEL_BINDING);
    tebo->bindBase(constants::TEXTURE_BINDING);
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
    const auto textureBuffer = m_mngr->get(m_textureBuffer);
    drawIndirectBuffer->invalidate();
    transformsBuffer->invalidate();
    textureBuffer->invalidate();

    auto cmdData = drawIndirectBuffer->getPtr<MultiDrawElementsIndirectCommand>();
    auto transformsData = transformsBuffer->getPtr<glm::mat3x4>();
    auto textureData = textureBuffer->getPtr<GLuint64>();

    GLuint firstIndex = 0;
    GLuint baseVertex = 0;
    GLuint baseInstance = 0;
    GLuint cmdIdx = 0;
    GLuint transformIdx = 0;
    GLuint textureIdx = 0;

    for (const auto& task : m_tasks)
    {
        const Model* model = m_mngr->get<Model>(task.model);

        vbo->pushData(model->vertices().data(), model->numVertices());
        ebo->pushData(model->indices().data(), model->numIndices());

        MultiDrawElementsIndirectCommand cmd{
            .vertexCount = static_cast<GLuint>(model->numIndices()),
            .instanceCount = task.instanceCount,
            .firstIndex = firstIndex,
            .baseVertex = baseVertex,
            .baseInstance = baseInstance
        };
        cmdData[cmdIdx++] = cmd;

        for (const auto& transform : task.transformations)
            transformsData[transformIdx++] = transform;

        for (const auto& tex : task.textures)
            textureData[textureIdx++] = m_mngr->get(tex)->getHandle();

        firstIndex += model->numIndices();
        baseVertex += model->numVertices();
        baseInstance += task.instanceCount;
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, nullptr, m_tasks.size(), 0);
}

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
