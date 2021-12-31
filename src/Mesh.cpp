#include "Mesh.hpp"

//------------------------------------------------------------------------

Mesh::Mesh(const std::vector<Vertex>& vertices,
    const std::vector<GLuint>& indices,
    std::unique_ptr<Settings> settings,
    GLenum usage,
    GLenum mode)
    : m_settings{std::move(settings)},
      m_nofVertices{vertices.size()},
      m_nofIndices{indices.size()}
{
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);

    glBindVertexArray(m_VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), usage);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const GLvoid*>(offsetof(Vertex, position)));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const GLvoid*>(offsetof(Vertex, normal)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const GLvoid*>(offsetof(Vertex, texCoords)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), usage);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

//------------------------------------------------------------------------

Mesh::~Mesh()
{
    if (glIsVertexArray(m_VAO) != GL_FALSE)
    {
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers(1, &m_VBO);
        glDeleteBuffers(1, &m_EBO);
    }
}

//------------------------------------------------------------------------

void Mesh::render() const noexcept
{
    pushMatrices();
    pushMaterial();
    
    m_settings->renderStrategy->use();
    m_settings->colorTexture->bind();

    glBindVertexArray(m_VAO);
    if (m_settings->indexed)
        glDrawElements(m_settings->mode, m_nofIndices, GL_UNSIGNED_INT, nullptr);
    else
        glDrawArrays(m_settings->mode, 0, m_nofVertices);
        
    m_settings->colorTexture->unbind();
    m_settings->renderStrategy->unuse();
}

//------------------------------------------------------------------------

void Mesh::pushMatrices() const noexcept
{
    const auto& M = m_settings->transformation;
    m_settings->renderStrategy->setUniform<glm::mat4>("u_M", glm::value_ptr(M));
    m_settings->renderStrategy->setUniform<glm::mat4>("u_N", glm::value_ptr(glm::inverseTranspose(M)));
}

//------------------------------------------------------------------------

void Mesh::pushMaterial() const noexcept
{
    m_settings->renderStrategy->setUniform<glm::vec3>("u_material.emission", glm::value_ptr(m_settings->material->emission));
    m_settings->renderStrategy->setUniform<glm::vec3>("u_material.ambient", glm::value_ptr(m_settings->material->ambient));
    m_settings->renderStrategy->setUniform<glm::vec3>("u_material.diffuse", glm::value_ptr(m_settings->material->diffuse));
    m_settings->renderStrategy->setUniform<glm::vec3>("u_material.specular", glm::value_ptr(m_settings->material->specular));
    m_settings->renderStrategy->setUniform<GLfloat>("u_material.shininess", &m_settings->material->shininess);
}

//------------------------------------------------------------------------
