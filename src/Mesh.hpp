#pragma once

#include "ShaderProgram.hpp"
#include "Texture2D.hpp"
#include "Util.hpp"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include <iostream>
#include <memory>
#include <string>
#include <vector>

//------------------------------------------------------------------------

class Mesh {
public:
    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texCoords;
    };
    struct Material {
        // Defaults to silver, no emission. See http://devernay.free.fr/cours/opengl/materials.html
        glm::vec3 emission = glm::vec3(0.0f);
        glm::vec3 ambient  = glm::vec3(0.19225f);
        glm::vec3 diffuse  = glm::vec3(0.50754f);
        glm::vec3 specular = glm::vec3(0.508273f);
        GLfloat shininess  = 0.4f;

        Material() = default;
    };

    Mesh(const std::vector<Vertex>& vertices,
         const std::vector<GLuint>& indices,
         const std::shared_ptr<ShaderProgram>& shaderProgram,
         const std::shared_ptr<Material>& material = std::make_shared<Material>(),
         GLenum draw_mode = GL_STATIC_DRAW);
    ~Mesh();

    inline void setMaterial(std::shared_ptr<Material> material) noexcept    { m_material = material; }
    inline void setTransformation(const glm::mat4& transformation) noexcept { m_transformation = transformation; }
    inline void setPrimitiveMode(GLenum glPrimitiveMode) noexcept           { m_primitiveMode = glPrimitiveMode; }

    inline const std::shared_ptr<Material>& getMaterial() const noexcept { return m_material; }
    inline const glm::mat4& getTransformation() const noexcept           { return m_transformation; }
    inline GLenum getPrimitiveMode() const noexcept                      { return m_primitiveMode; }

    void render() const noexcept;
    void pushModelMatrix() const noexcept;
    void pushMaterial() const noexcept;

    static std::unique_ptr<Mesh> fromFile(const std::string& filename,
                                          const std::shared_ptr<ShaderProgram>& shader_program,
                                          const std::shared_ptr<Material>& material = std::make_shared<Material>(),
                                          GLenum draw_mode = GL_STATIC_DRAW);

    static std::unique_ptr<Mesh> makeCube(const std::shared_ptr<ShaderProgram>& shader_program,
                                          const std::shared_ptr<Material>& material = std::make_shared<Material>(),
                                          GLenum draw_mode = GL_STATIC_DRAW);

    static std::unique_ptr<Mesh> makePlane(const std::shared_ptr<ShaderProgram>& shader_program,
                                           const std::shared_ptr<Material>& material = std::make_shared<Material>(),
                                           GLenum draw_mode = GL_STATIC_DRAW);

private:
    GLuint m_VAO = 0;
    GLuint m_VBO = 0;
    GLuint m_EBO = 0;
    GLenum m_drawMode = GL_STATIC_DRAW;
    GLenum m_primitiveMode = GL_TRIANGLES;
    GLsizei m_nofIndices = 0;

    std::shared_ptr<Material> m_material = nullptr;
    std::shared_ptr<Texture2D> m_colorTexture = std::shared_ptr<Texture2D>(Texture2D::makeDefault());
    glm::mat4 m_transformation = glm::mat4(1.0f);
    std::shared_ptr<ShaderProgram> m_shaderProgram;
};

//------------------------------------------------------------------------
