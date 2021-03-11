#pragma once

#include "ShaderProgram.hpp"
#include "Util.hpp"

extern "C" {
#include <GL/glew.h>
}
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include <iostream>
#include <memory>
#include <string>
#include <vector>


class Mesh {
public:
    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;

        Vertex(const glm::vec3& position,
               const glm::vec3& normal)
            : position(position), normal(normal) {}
    };
    struct Material {
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
        GLfloat shininess;

        Material(const glm::vec3& ambient,
                 const glm::vec3& diffuse,
                 const glm::vec3& specular,
                 GLfloat shininess)
            : ambient(ambient), diffuse(diffuse), specular(specular),
              shininess(shininess) {}

        static std::unique_ptr<Material> make_default()
        {
            // Silver, see http://devernay.free.fr/cours/opengl/materials.html
            return std::make_unique<Material>(
                glm::vec3(0.19225f),
                glm::vec3(0.50754f),
                glm::vec3(0.508273f),
                0.4f
            );
        }
    };

    Mesh(const std::vector<Vertex>& vertices,
         const std::vector<GLuint>& indices,
         std::shared_ptr<ShaderProgram> shader_program,
         std::shared_ptr<Material> material = std::move(Material::make_default()),
         GLenum draw_mode = GL_STATIC_DRAW);
    Mesh(const Mesh&) = delete;
    ~Mesh();

    Mesh& operator=(const Mesh&) = delete;

    void render();

    inline void set_material(std::shared_ptr<Material> material)    { m_material = std::move(material); }
    inline void set_transformation(const glm::mat4& transformation) { m_transformation = transformation; }
    inline void set_primitive_mode(GLenum gl_primitive_mode)        { m_primitive_mode = gl_primitive_mode; }

    inline const std::shared_ptr<Material>& get_material() const { return m_material; }
    inline const glm::mat4& get_transformation() const           { return m_transformation; }
    inline GLenum get_primitive_mode() const                     { return m_primitive_mode; }

    void push_model_matrix();
    void push_material();

    static std::unique_ptr<Mesh> from_file(const std::string& filename,
                                           std::shared_ptr<ShaderProgram> shader_program,
                                           std::shared_ptr<Material> material = std::move(Material::make_default()),
                                           GLenum draw_mode = GL_STATIC_DRAW);

    static std::unique_ptr<Mesh> make_cube(std::shared_ptr<ShaderProgram> shader_program,
                                           std::shared_ptr<Material> material = std::move(Material::make_default()),
                                           GLenum draw_mode = GL_STATIC_DRAW);

    static std::unique_ptr<Mesh> make_plane(std::shared_ptr<ShaderProgram> shader_program,
                                            std::shared_ptr<Material> material = std::move(Material::make_default()),
                                            GLenum draw_mode = GL_STATIC_DRAW);

private:
    GLuint m_vao = 0;
    GLuint m_vbo = 0;
    GLuint m_ebo = 0;
    GLenum m_draw_mode = GL_STATIC_DRAW;
    GLenum m_primitive_mode = GL_TRIANGLES;
    GLsizei m_nof_indices = 0;

    std::shared_ptr<Material> m_material = nullptr;
    glm::mat4 m_transformation = glm::mat4(1.0f);
    std::shared_ptr<ShaderProgram> m_shader_program;
};