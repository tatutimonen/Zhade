#pragma once

#include "ShaderProgram.hpp"
#include "Texture2D.hpp"
#include "util.hpp"

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
        GLenum drawMode                               = GL_STATIC_DRAW;
        GLenum primitiveMode                          = GL_TRIANGLES;

        Settings() = default;
    };

    Mesh(const std::vector<Vertex>& vertices,
        const std::vector<GLuint>& indices,
        std::unique_ptr<Settings> settings = std::make_unique<Settings>());
    ~Mesh();

    inline const Material& getMaterial() const noexcept        { return *m_settings->material; }
    inline const glm::mat4& getTransformation() const noexcept { return m_settings->transformation; }
    inline GLenum getPrimitiveMode() const noexcept            { return m_settings->primitiveMode; }

    inline void setMaterial(const std::shared_ptr<Material>& material) noexcept { m_settings->material = material; }
    inline void setTransformation(const glm::mat4& transformation) noexcept     { m_settings->transformation = transformation; }
    inline void setPrimitiveMode(GLenum glPrimitiveMode) noexcept               { m_settings->primitiveMode = glPrimitiveMode; }

    void render() const noexcept;
    void pushModelMatrix() const noexcept;
    void pushMaterial() const noexcept;

private:
    GLuint m_VAO = 0;
    GLuint m_VBO = 0;
    GLuint m_EBO = 0;
    std::size_t m_nofIndices = 0;

    std::unique_ptr<Settings> m_settings;
};

//------------------------------------------------------------------------
