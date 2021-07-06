#include "Mesh.hpp"

//------------------------------------------------------------------------

Mesh::Mesh(const std::vector<Vertex>& vertices,
           const std::vector<GLuint>& indices,
           const std::shared_ptr<ShaderProgram>& shaderProgram,
           const std::shared_ptr<Material>& material,
           const std::shared_ptr<Texture2D>& colorTexture,
           GLenum drawMode)
    : m_shaderProgram{shaderProgram},
      m_material{material},
      m_colorTexture{colorTexture},
      m_drawMode{drawMode},
      m_nofIndices{static_cast<GLsizei>(indices.size())}
{
    GL_CALL(glGenVertexArrays(1, &m_VAO));
    GL_CALL(glGenBuffers(1, &m_VBO));
    GL_CALL(glGenBuffers(1, &m_EBO));

    GL_CALL(glBindVertexArray(m_VAO));
    
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_VBO));
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(Vertex), vertices.data(), m_drawMode));

    GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)0));
    GL_CALL(glEnableVertexAttribArray(0));
    GL_CALL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)offsetof(Vertex, normal)));
    GL_CALL(glEnableVertexAttribArray(1));
    GL_CALL(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)offsetof(Vertex, texCoords)));
    GL_CALL(glEnableVertexAttribArray(2));

    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO));
    GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(GLuint), indices.data(), m_drawMode));

    GL_CALL(glBindVertexArray(0));
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

//------------------------------------------------------------------------

Mesh::~Mesh()
{
    GL_CALL(glDeleteVertexArrays(1, &m_VAO));
    GL_CALL(glDeleteBuffers(1, &m_VBO));
    GL_CALL(glDeleteBuffers(1, &m_EBO));
}

//------------------------------------------------------------------------

void Mesh::render() const noexcept
{
    pushMaterial();
    pushModelMatrix();
    
    m_shaderProgram->use();
    m_colorTexture->bind();
    GL_CALL(glBindVertexArray(m_VAO));
    GL_CALL(glDrawElements(m_primitiveMode, m_nofIndices, GL_UNSIGNED_INT, (const GLvoid*)0));
    m_colorTexture->unbind();
    m_shaderProgram->unuse();
}

//------------------------------------------------------------------------

void Mesh::pushModelMatrix() const noexcept
{
    m_shaderProgram->setUniform<glm::mat4>("u_model", glm::value_ptr(m_transformation));
}

//------------------------------------------------------------------------

void Mesh::pushMaterial() const noexcept
{
    m_shaderProgram->setUniform<glm::vec3>("u_material.emission", glm::value_ptr(m_material->emission));
    m_shaderProgram->setUniform<glm::vec3>("u_material.ambient", glm::value_ptr(m_material->ambient));
    m_shaderProgram->setUniform<glm::vec3>("u_material.diffuse", glm::value_ptr(m_material->diffuse));
    m_shaderProgram->setUniform<glm::vec3>("u_material.specular", glm::value_ptr(m_material->specular));
    m_shaderProgram->setUniform<GLfloat>("u_material.shininess", &m_material->shininess);
}

//------------------------------------------------------------------------

std::unique_ptr<Mesh> Mesh::fromFile(const std::string& filename,
                                     const std::shared_ptr<ShaderProgram>& shaderProgram,
                                     const std::shared_ptr<Material>& material,
                                     GLenum drawMode)
{
    return nullptr;
}

//------------------------------------------------------------------------

std::unique_ptr<Mesh> Mesh::makeCube(const std::shared_ptr<ShaderProgram>& shaderProgram,
                                     const std::shared_ptr<Material>& material,
                                     const std::shared_ptr<Texture2D>& colorTexture,
                                     GLenum drawMode)
{
    const std::vector<Vertex> vertices{
        // bottom
        { glm::vec3( 0.5f,  0.0f,  0.5f),  Util::makeUnitVec3z(), glm::vec2() },
        { glm::vec3(-0.5f,  0.0f,  0.5f), -Util::makeUnitVec3x(), glm::vec2() },
        { glm::vec3(-0.5f,  0.0f, -0.5f), -Util::makeUnitVec3z(), glm::vec2() },
        { glm::vec3( 0.5f,  0.0f, -0.5f),  Util::makeUnitVec3x(), glm::vec2() },
        // top
        { glm::vec3( 0.5f,  1.0f,  0.5f),  Util::makeUnitVec3z(), glm::vec2() },
        { glm::vec3(-0.5f,  1.0f,  0.5f), -Util::makeUnitVec3x(), glm::vec2() },
        { glm::vec3(-0.5f,  1.0f, -0.5f), -Util::makeUnitVec3z(), glm::vec2() },
        { glm::vec3( 0.5f,  1.0f, -0.5f),  Util::makeUnitVec3x(), glm::vec2() }
    };

    const std::vector<GLuint> indices{
        // xz-parallel sides
        0, 1, 2, 2, 3, 0,
        4, 5, 6, 6, 7, 4,
        // y-parallel sides
        0, 1, 4, 4, 5, 1,
        1, 2, 5, 5, 6, 2,
        2, 3, 6, 6, 7, 3,
        3, 0, 7, 7, 4, 0
    };

    return std::make_unique<Mesh>(vertices, indices, shaderProgram, material, colorTexture, drawMode);
}

//------------------------------------------------------------------------

std::unique_ptr<Mesh> Mesh::makePlane(const std::shared_ptr<ShaderProgram>& shaderProgram,
                                      const std::shared_ptr<Material>& material,
                                      const std::shared_ptr<Texture2D>& colorTexture,
                                      GLenum drawMode)
{
    const std::vector<Vertex> vertices{
        { glm::vec3( 0.5f,  0.0f,  0.5f), Util::makeUnitVec3y(), glm::vec2(1.0f, 1.0f) },
        { glm::vec3(-0.5f,  0.0f,  0.5f), Util::makeUnitVec3y(), glm::vec2(0.0f, 1.0f) },
        { glm::vec3(-0.5f,  0.0f, -0.5f), Util::makeUnitVec3y(), glm::vec2(0.0f, 0.0f) },
        { glm::vec3( 0.5f,  0.0f, -0.5f), Util::makeUnitVec3y(), glm::vec2(1.0f, 0.0f) }
    };
    
    const std::vector<GLuint> indices{
        0, 1, 2, 2, 3, 0
    };

    return std::make_unique<Mesh>(vertices, indices, shaderProgram, material, colorTexture, drawMode);
}

//------------------------------------------------------------------------
