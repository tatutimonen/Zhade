#pragma once

#include "App.hpp"
#include "ShaderProgram.hpp"
#include "Texture2D.hpp"
#include "UniformBuffer.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "constants.hpp"
#include "util.hpp"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtx/string_cast.hpp>

#include <cstdint>
#include <iostream>
#include <memory>
#include <numeric>
#include <string>
#include <vector>

//------------------------------------------------------------------------

class Mesh {
public:
    struct Vertex {
        glm::vec3 position;
        uint32_t normal;
        glm::vec2 texCoords;  // TODO: half-floats
    };
    struct Material {
        glm::vec3 ambient  = glm::vec3(1.0f);
        float _1           = constants::STD_140_PAD_FLOAT;
        glm::vec3 diffuse  = glm::vec3(1.0f);
        float _2           = constants::STD_140_PAD_FLOAT;
        glm::vec3 specular = glm::vec3(1.0f);
        float shininess    = 0.4f;

        Material(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, float shininess)
            : ambient{ambient},
              diffuse{diffuse},
              specular{specular},
              shininess{shininess}
        {}
    };
    struct Settings {
        std::shared_ptr<Material> material               = std::make_shared<Material>();
        std::shared_ptr<const Texture2D> colorTexture    = App::getInstance().defaultTexture2D;
        std::shared_ptr<const UniformBuffer> uboMaterial = std::make_shared<const UniformBuffer>();
        std::shared_ptr<const UniformBuffer> uboMatrices = std::make_shared<const UniformBuffer>();
        glm::mat4 M                                      = glm::mat4(1.0f);
        glm::mat4 N                                      = glm::mat4(1.0f);
        uint32_t mode                                    = GL_TRIANGLES;
        bool indexed                                     = false;
    };

    Mesh(const std::vector<Vertex>& vertices, std::shared_ptr<const VertexArray> vertexArray,
        const Settings& settings = Settings(), const std::vector<uint32_t>& indices = {}, uint32_t usage = GL_STATIC_DRAW);

    inline const Settings& getSettings() const noexcept
    {
        return m_settings;
    }

    inline void setMaterial(std::shared_ptr<Material> material) noexcept
    {
        m_settings.material = material;
    }

    inline void setModelMatrix(const glm::mat4& M) noexcept
    {
        m_settings.M = M;
        m_settings.N = glm::inverseTranspose(M);
    }

    void render(bool colors = true) const noexcept;

private:
    float computeAverageZ(const std::vector<Vertex>& vertices);
    void uploadMatrices() const noexcept;
    void uploadMaterial() const noexcept;

    Settings m_settings;
    uint32_t m_nofVertices;
    VertexBuffer m_vertexBuffer;
    std::shared_ptr<const VertexArray> m_vertexArray;
    float zAvg;
};

//------------------------------------------------------------------------
