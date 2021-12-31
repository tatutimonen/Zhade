#pragma once

#include "constants.hpp"
#include "ShaderProgram.hpp"
#include "Texture2D.hpp"
#include "util.hpp"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
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
        glm::vec3 emission = glm::vec3(0.0f);
        glm::vec3 ambient  = glm::vec3(1.0f);
        glm::vec3 diffuse  = glm::vec3(1.0f);
        glm::vec3 specular = glm::vec3(1.0f);
        GLfloat shininess  = 0.4f;

        Material() = default;
    };
    struct Settings {
        std::shared_ptr<ShaderProgram> renderStrategy = std::make_shared<ShaderProgram>();
        std::shared_ptr<Material> material            = std::make_shared<Material>();
        std::shared_ptr<Texture2D> colorTexture       = std::shared_ptr<Texture2D>(Texture2D::makeDefault());
        glm::mat4 transformation                      = glm::mat4(1.0f);
        GLenum mode                                   = GL_TRIANGLES;
        bool indexed                                  = true;

        Settings() = default;
    };

    Mesh(const std::vector<Vertex>& vertices,
        const std::vector<GLuint>& indices,
        std::unique_ptr<Settings> settings = std::make_unique<Settings>(),
        GLenum usage = GL_STATIC_DRAW,
        GLenum mode = GL_TRIANGLES);
    ~Mesh();

    inline const Material& getMaterial() const noexcept        { return *m_settings->material; }
    inline const glm::mat4& getTransformation() const noexcept { return m_settings->transformation; }

    inline void setMaterial(const std::shared_ptr<Material>& material) noexcept { m_settings->material = material; }
    inline void setTransformation(const glm::mat4& transformation) noexcept     { m_settings->transformation = transformation; }

    void render() const noexcept;
    void pushMatrices() const noexcept;
    void pushMaterial() const noexcept;

private:
    GLuint m_vao = 0;
    GLuint m_vbo = 0;
    GLuint m_ebo = 0;

    std::unique_ptr<Settings> m_settings;
    std::size_t m_nofVertices;
    std::size_t m_nofIndices;
};

//------------------------------------------------------------------------
