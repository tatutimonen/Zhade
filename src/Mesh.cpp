#include "Mesh.hpp"

//------------------------------------------------------------------------

Mesh::Mesh(const std::vector<Vertex>& vertices, std::shared_ptr<const VertexArray> vertexArray,
    const Settings& settings, const std::vector<uint32_t>& indices, uint32_t usage)
    : m_settings{settings},
      m_nofVertices{vertices.size()},
      m_vertexBuffer{VertexBuffer(sizeof(Vertex) * vertices.size(), usage, vertices.data())},
      zAvg{computeAverageZ(vertices)}
{
    // Compute AABB...
}

//------------------------------------------------------------------------

void Mesh::render(bool colors) const noexcept
{
    uploadMatrices();
    if (colors)
        uploadMaterial();
    
    m_settings.colorTexture->bind();
    
    m_vertexArray->bind();
    m_vertexBuffer.bind(sizeof(Vertex));
    glDrawArrays(m_settings.mode, 0, m_nofVertices);
}

//------------------------------------------------------------------------

void Mesh::uploadMatrices() const noexcept
{
    m_settings.uboMatrices->update(0, reinterpret_cast<const void*>(offsetof(Settings, M)), 2 * sizeof(glm::mat4));
}

//------------------------------------------------------------------------

void Mesh::uploadMaterial() const noexcept
{
    m_settings.uboMaterial->update(0, m_settings.material.get(), sizeof(Material));
}

//------------------------------------------------------------------------

float Mesh::computeAverageZ(const std::vector<Vertex>& vertices)
{
    auto result = 0.0f;
    for (const auto& vertex : vertices)
        result += vertex.position.z;
    return result / vertices.size();
}

//------------------------------------------------------------------------
