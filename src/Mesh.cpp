#include "Mesh.hpp"

//------------------------------------------------------------------------

Mesh::Mesh(const std::vector<Vertex>& vertices,
    const std::vector<GLuint>& indices,
    std::unique_ptr<Settings> settings)
    : m_settings{std::move(settings)},
      m_nofIndices{static_cast<GLsizei>(indices.size())}
{
    std::cout << "ctor: " << this << std::endl;
    CHECK_GL_ERROR(glGenVertexArrays(1, &m_VAO));
    CHECK_GL_ERROR(glGenBuffers(1, &m_VBO));
    CHECK_GL_ERROR(glGenBuffers(1, &m_EBO));

    CHECK_GL_ERROR(glBindVertexArray(m_VAO));
    
    CHECK_GL_ERROR(glBindBuffer(GL_ARRAY_BUFFER, m_VBO));
    CHECK_GL_ERROR(glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(Vertex), vertices.data(), m_settings->drawMode));

    CHECK_GL_ERROR(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)0));
    CHECK_GL_ERROR(glEnableVertexAttribArray(0));
    CHECK_GL_ERROR(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)offsetof(Vertex, normal)));
    CHECK_GL_ERROR(glEnableVertexAttribArray(1));
    CHECK_GL_ERROR(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)offsetof(Vertex, texCoords)));
    CHECK_GL_ERROR(glEnableVertexAttribArray(2));

    CHECK_GL_ERROR(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO));
    CHECK_GL_ERROR(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(GLuint), indices.data(), m_settings->drawMode));

    CHECK_GL_ERROR(glBindVertexArray(0));
    CHECK_GL_ERROR(glBindBuffer(GL_ARRAY_BUFFER, 0));
    CHECK_GL_ERROR(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

//------------------------------------------------------------------------

Mesh::~Mesh()
{
    std::cout << "dtor: " << m_VAO << " " << this << std::endl;
    if (glIsVertexArray(m_VAO) != GL_FALSE)
    {
        CHECK_GL_ERROR(glDeleteVertexArrays(1, &m_VAO));
        CHECK_GL_ERROR(glDeleteBuffers(1, &m_VBO));
        CHECK_GL_ERROR(glDeleteBuffers(1, &m_EBO));
    }
}

//------------------------------------------------------------------------

void Mesh::render() const noexcept
{
    pushMaterial();
    pushModelMatrix();
    
    m_settings->renderStrategy->use();
    m_settings->colorTexture->bind();
    CHECK_GL_ERROR(glBindVertexArray(m_VAO));
    CHECK_GL_ERROR(glDrawElements(m_settings->primitiveMode, m_nofIndices, GL_UNSIGNED_INT, (const GLvoid*)0));
    m_settings->colorTexture->unbind();
    m_settings->renderStrategy->unuse();
}

//------------------------------------------------------------------------

void Mesh::pushModelMatrix() const noexcept
{
    m_settings->renderStrategy->setUniform<glm::mat4>("u_MMatrix", glm::value_ptr(m_settings->transformation));
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
