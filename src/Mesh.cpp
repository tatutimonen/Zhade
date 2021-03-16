#include "Mesh.hpp"


Mesh::Mesh(const std::vector<Vertex>& vertices,
           const std::vector<GLuint>& indices,
           std::shared_ptr<ShaderProgram> shader_program,
           std::shared_ptr<Material> material,
           GLenum draw_mode)
    : m_material(std::move(material)), m_draw_mode(draw_mode), m_shader_program(std::move(shader_program)),
      m_nof_indices(indices.size())
{
    GL_CALL(glGenVertexArrays(1, &m_vao));
    GL_CALL(glGenBuffers(1, &m_vbo));
    GL_CALL(glGenBuffers(1, &m_ebo));

    GL_CALL(glBindVertexArray(m_vao));
    
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_vbo));
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(Vertex), vertices.data(), m_draw_mode));

    GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)0));
    GL_CALL(glEnableVertexAttribArray(0));
    GL_CALL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)offsetof(Vertex, normal)));
    GL_CALL(glEnableVertexAttribArray(1));

    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo));
    GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(GLuint), indices.data(), m_draw_mode));

    GL_CALL(glBindVertexArray(0));
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

Mesh::~Mesh()
{
    GL_CALL(glDeleteVertexArrays(1, &m_vao));
    GL_CALL(glDeleteBuffers(1, &m_vbo));
    GL_CALL(glDeleteBuffers(1, &m_ebo));
}

void Mesh::render()
{
    push_material();
    push_model_matrix();
    m_shader_program->use();
    GL_CALL(glBindVertexArray(m_vao));
    GL_CALL(glDrawElements(m_primitive_mode, m_nof_indices, GL_UNSIGNED_INT, (const GLvoid*)0));
    GL_CALL(glBindVertexArray(0));
    m_shader_program->unuse();
}

void Mesh::push_model_matrix()
{
    m_shader_program->set_uniform<glm::mat4>("model", glm::value_ptr(m_transformation));
}

void Mesh::push_material()
{
    m_shader_program->set_uniform<glm::vec3>("material.ambient", glm::value_ptr(m_material->ambient));
    m_shader_program->set_uniform<glm::vec3>("material.diffuse", glm::value_ptr(m_material->diffuse));
    m_shader_program->set_uniform<glm::vec3>("material.specular", glm::value_ptr(m_material->specular));
    m_shader_program->set_uniform<GLfloat>("material.shininess", &m_material->shininess);
}

std::unique_ptr<Mesh> Mesh::from_file(const std::string& filename,
                                      std::shared_ptr<ShaderProgram> shader_program,
                                      std::shared_ptr<Material> material,
                                      GLenum draw_mode)
{
    return nullptr;
}

std::unique_ptr<Mesh> Mesh::make_cube(std::shared_ptr<ShaderProgram> shader_program,
                                      std::shared_ptr<Material> material,
                                      GLenum draw_mode)
{
    const std::vector<Vertex> vertices = {
        // bottom
        { glm::vec3( 0.5f,  0.0f,  0.5f),  Util::make_unit_vec3z() },
        { glm::vec3(-0.5f,  0.0f,  0.5f), -Util::make_unit_vec3x() },
        { glm::vec3(-0.5f,  0.0f, -0.5f), -Util::make_unit_vec3z() },
        { glm::vec3( 0.5f,  0.0f, -0.5f),  Util::make_unit_vec3x() },
        // top
        { glm::vec3( 0.5f,  1.0f,  0.5f),  Util::make_unit_vec3z() },
        { glm::vec3(-0.5f,  1.0f,  0.5f), -Util::make_unit_vec3x() },
        { glm::vec3(-0.5f,  1.0f, -0.5f), -Util::make_unit_vec3z() },
        { glm::vec3( 0.5f,  1.0f, -0.5f),  Util::make_unit_vec3x() }
    };

    const std::vector<GLuint> indices = {
        // xz-parallel sides
        0, 1, 2, 2, 3, 0,
        4, 5, 6, 6, 7, 4,
        // y-parallel sides
        0, 1, 4, 4, 5, 1,
        1, 2, 5, 5, 6, 2,
        2, 3, 6, 6, 7, 3,
        3, 0, 7, 7, 4, 0
    };

    return std::make_unique<Mesh>(vertices, indices, shader_program, material, draw_mode);
}

std::unique_ptr<Mesh> Mesh::make_plane(std::shared_ptr<ShaderProgram> shader_program,
                                       std::shared_ptr<Material> material,
                                       GLenum draw_mode)
{
    const std::vector<Vertex> vertices = {
        { glm::vec3( 0.5f,  0.0f,  0.5f), Util::make_unit_vec3y() },
        { glm::vec3(-0.5f,  0.0f,  0.5f), Util::make_unit_vec3y() },
        { glm::vec3(-0.5f,  0.0f, -0.5f), Util::make_unit_vec3y() },
        { glm::vec3( 0.5f,  0.0f, -0.5f), Util::make_unit_vec3y() }
    };
    
    const std::vector<GLuint> indices = {
        0, 1, 2, 2, 3, 0
    };

    return std::make_unique<Mesh>(vertices, indices, shader_program, material, draw_mode);
}