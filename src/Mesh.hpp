#pragma once

#include "AttributeData.hpp"
#include "VertexFormat.hpp"
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
#include <string>
#include <vector>

//------------------------------------------------------------------------

class Mesh {
public:
    struct Settings {
        std::shared_ptr<Material> material      = std::make_shared<Material>();
        std::shared_ptr<Texture2D> colorTexture = Texture2D::makeDefault();
        Matrices matrices                       = Matrices();
        uint32_t mode                           = GL_TRIANGLES;
    };

    Mesh(const std::vector<Vertex>& vertices, std::shared_ptr<const VertexArray> vertexArray, const Settings& settings = Settings(),
        const std::vector<uint32_t>& indices = {}, uint32_t usage = GL_STATIC_DRAW);

    const Settings& getSettings() const noexcept
    {
        return m_settings;
    }

    void setMaterial(std::shared_ptr<Material> material) noexcept
    {
        m_settings.material = material;
    }

    void setModelMatrix(const glm::mat4& M) noexcept
    {
        m_settings.matrices.M = M;
        m_settings.matrices.N = glm::inverseTranspose(M);
    }

    void render(bool colors = true) const noexcept;

private:
    float computeAverageZ(const std::vector<Vertex>& vertices);
    void uploadMatrices() const noexcept;
    void uploadMaterial() const noexcept;

    Settings m_settings;
    std::shared_ptr<const VertexArray> m_vertexArray;
    VertexBuffer m_vertexBuffer;
    std::size_t m_nofVertices;
    UniformBuffer m_uniformBufferMaterial;
    UniformBuffer m_uniformBufferMatrices;
    uint32_t m_ebobj = 0;
    std::size_t m_nofIndices = 0;
    float m_zAvg;
};

//------------------------------------------------------------------------
