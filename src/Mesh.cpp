#include "Mesh.hpp"
#include <glm/gtx/string_cast.hpp>

//------------------------------------------------------------------------

Mesh::Mesh(const std::vector<Vertex>& vertices, std::shared_ptr<const VertexArray> vertexArray, const Settings& settings,
    const std::vector<uint32_t>& indices, uint32_t usage)
    : m_settings{settings},
      m_vertexArray{vertexArray},
      m_vertexBuffer{VertexBuffer(sizeof(Vertex) * vertices.size(), usage, vertices.data())},
      m_nofVertices{vertices.size()},
      m_uniformBufferMaterial{UniformBuffer("Material", constants::MATERIAL_BINDING, sizeof(Material))},
      m_uniformBufferMatrices{UniformBuffer("Model", constants::MODEL_BINDING, sizeof(Matrices))},
      m_nofIndices{indices.size()},
      m_zAvg{computeAverageZ(vertices)}
{
    if (m_nofIndices > 0)
    {
        glGenBuffers(1, &m_ebobj);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebobj);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * indices.size(), indices.data(), usage);
    }
}

//------------------------------------------------------------------------

void Mesh::render(bool colors) const noexcept
{
    uploadMatrices();
    uploadMaterial();
    
    m_vertexBuffer.bind(sizeof(Vertex));
    m_settings.colorTexture->bind();
    if (m_nofIndices > 0)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebobj);
        glDrawElements(m_settings.mode, m_nofIndices, GL_UNSIGNED_INT, nullptr);
    }
    else
    {
        glDrawArrays(m_settings.mode, 0, m_nofVertices);
    }
}

//------------------------------------------------------------------------

void Mesh::uploadMatrices() const noexcept
{
    m_uniformBufferMatrices.update(0, &m_settings.matrices, sizeof(Matrices));
}

//------------------------------------------------------------------------

void Mesh::uploadMaterial() const noexcept
{
    m_uniformBufferMaterial.update(0, m_settings.material.get(), sizeof(Material));
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
