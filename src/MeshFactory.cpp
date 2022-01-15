#include "MeshFactory.hpp"

//------------------------------------------------------------------------

std::unique_ptr<Mesh> MeshFactory::makeCube(const Mesh::Settings& settings)
{
    static const std::vector<Mesh::Vertex> vertices = {
        // Bottom.
        { glm::vec3( 0.5f,  0.0f,  0.5f), util::makeNegUnitVec3yPacked(), glm::vec2(1.0f, 1.0f) },
        { glm::vec3(-0.5f,  0.0f,  0.5f), util::makeNegUnitVec3yPacked(), glm::vec2(0.0f, 1.0f) },
        { glm::vec3(-0.5f,  0.0f, -0.5f), util::makeNegUnitVec3yPacked(), glm::vec2(0.0f, 0.0f) },
        { glm::vec3(-0.5f,  0.0f, -0.5f), util::makeNegUnitVec3yPacked(), glm::vec2(0.0f, 0.0f) },
        { glm::vec3( 0.5f,  0.0f, -0.5f), util::makeNegUnitVec3yPacked(), glm::vec2(1.0f, 0.0f) },
        { glm::vec3( 0.5f,  0.0f,  0.5f), util::makeNegUnitVec3yPacked(), glm::vec2(1.0f, 1.0f) },
        // Top.
        { glm::vec3( 0.5f,  1.0f,  0.5f),    util::makeUnitVec3yPacked(), glm::vec2(1.0f, 0.0f) },
        { glm::vec3( 0.5f,  1.0f, -0.5f),    util::makeUnitVec3yPacked(), glm::vec2(1.0f, 1.0f) },
        { glm::vec3(-0.5f,  1.0f, -0.5f),    util::makeUnitVec3yPacked(), glm::vec2(0.0f, 1.0f) },
        { glm::vec3(-0.5f,  1.0f, -0.5f),    util::makeUnitVec3yPacked(), glm::vec2(0.0f, 1.0f) },
        { glm::vec3(-0.5f,  1.0f,  0.5f),    util::makeUnitVec3yPacked(), glm::vec2(0.0f, 0.0f) },
        { glm::vec3( 0.5f,  1.0f,  0.5f),    util::makeUnitVec3yPacked(), glm::vec2(1.0f, 0.0f) },
        // Right.
        { glm::vec3( 0.5f,  0.0f,  0.5f),    util::makeUnitVec3xPacked(), glm::vec2(0.0f, 0.0f) },
        { glm::vec3( 0.5f,  0.0f, -0.5f),    util::makeUnitVec3xPacked(), glm::vec2(1.0f, 0.0f) },
        { glm::vec3( 0.5f,  1.0f, -0.5f),    util::makeUnitVec3xPacked(), glm::vec2(1.0f, 1.0f) },
        { glm::vec3( 0.5f,  1.0f, -0.5f),    util::makeUnitVec3xPacked(), glm::vec2(1.0f, 1.0f) },
        { glm::vec3( 0.5f,  1.0f,  0.5f),    util::makeUnitVec3xPacked(), glm::vec2(0.0f, 1.0f) },
        { glm::vec3( 0.5f,  0.0f,  0.5f),    util::makeUnitVec3xPacked(), glm::vec2(0.0f, 0.0f) },
        // Back.
        { glm::vec3( 0.5f,  0.0f, -0.5f), util::makeNegUnitVec3zPacked(), glm::vec2(0.0f, 0.0f) },
        { glm::vec3(-0.5f,  0.0f, -0.5f), util::makeNegUnitVec3zPacked(), glm::vec2(1.0f, 0.0f) },
        { glm::vec3(-0.5f,  1.0f, -0.5f), util::makeNegUnitVec3zPacked(), glm::vec2(1.0f, 1.0f) },
        { glm::vec3(-0.5f,  1.0f, -0.5f), util::makeNegUnitVec3zPacked(), glm::vec2(1.0f, 1.0f) },
        { glm::vec3( 0.5f,  1.0f, -0.5f), util::makeNegUnitVec3zPacked(), glm::vec2(0.0f, 1.0f) },
        { glm::vec3( 0.5f,  0.0f, -0.5f), util::makeNegUnitVec3zPacked(), glm::vec2(0.0f, 0.0f) },
        // Left.
        { glm::vec3(-0.5f,  0.0f, -0.5f), util::makeNegUnitVec3xPacked(), glm::vec2(0.0f, 0.0f) },
        { glm::vec3(-0.5f,  0.0f,  0.5f), util::makeNegUnitVec3xPacked(), glm::vec2(1.0f, 0.0f) },
        { glm::vec3(-0.5f,  1.0f,  0.5f), util::makeNegUnitVec3xPacked(), glm::vec2(1.0f, 1.0f) },
        { glm::vec3(-0.5f,  1.0f,  0.5f), util::makeNegUnitVec3xPacked(), glm::vec2(1.0f, 1.0f) },
        { glm::vec3(-0.5f,  1.0f, -0.5f), util::makeNegUnitVec3xPacked(), glm::vec2(0.0f, 1.0f) },
        { glm::vec3(-0.5f,  0.0f, -0.5f), util::makeNegUnitVec3xPacked(), glm::vec2(0.0f, 0.0f) },
        // Front.
        { glm::vec3(-0.5f,  0.0f,  0.5f),    util::makeUnitVec3zPacked(), glm::vec2(0.0f, 0.0f) },
        { glm::vec3( 0.5f,  0.0f,  0.5f),    util::makeUnitVec3zPacked(), glm::vec2(1.0f, 0.0f) },
        { glm::vec3( 0.5f,  1.0f,  0.5f),    util::makeUnitVec3zPacked(), glm::vec2(1.0f, 1.0f) },
        { glm::vec3( 0.5f,  1.0f,  0.5f),    util::makeUnitVec3zPacked(), glm::vec2(1.0f, 1.0f) },
        { glm::vec3(-0.5f,  1.0f,  0.5f),    util::makeUnitVec3zPacked(), glm::vec2(0.0f, 1.0f) },
        { glm::vec3(-0.5f,  0.0f,  0.5f),    util::makeUnitVec3zPacked(), glm::vec2(0.0f, 0.0f) }
    };

    return std::make_unique<Mesh>(vertices, std::vector<uint32_t>(), settings);
}

//------------------------------------------------------------------------

std::unique_ptr<Mesh> MeshFactory::makePlane(const Mesh::Settings& settings)
{
    static const std::vector<Mesh::Vertex> vertices{
        { glm::vec3( 0.5f,  0.0f,  0.5f), util::makeUnitVec3yPacked(), glm::vec2(1.0f, 0.0f) },
        { glm::vec3( 0.5f,  0.0f, -0.5f), util::makeUnitVec3yPacked(), glm::vec2(1.0f, 1.0f) },
        { glm::vec3(-0.5f,  0.0f, -0.5f), util::makeUnitVec3yPacked(), glm::vec2(0.0f, 1.0f) },
        { glm::vec3(-0.5f,  0.0f,  0.5f), util::makeUnitVec3yPacked(), glm::vec2(0.0f, 0.0f) }
    };
    
    static const std::vector<uint32_t> indices{
        0, 1, 2, 2, 3, 0
    };

    return std::make_unique<Mesh>(vertices, indices, settings);
}

//------------------------------------------------------------------------
