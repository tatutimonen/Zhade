#include "MeshFactory.hpp"

//------------------------------------------------------------------------

std::unique_ptr<Mesh> MeshFactory::makeCube(std::unique_ptr<Mesh::Settings> settings)
{
    static const std::vector<Mesh::Vertex> vertices = {
        // Bottom.
        { glm::vec3( 0.5f,  0.0f,  0.5f), -util::makeUnitVec3y(), glm::vec2() },
        { glm::vec3( 0.5f,  0.0f, -0.5f), -util::makeUnitVec3y(), glm::vec2() },
        { glm::vec3(-0.5f,  0.0f, -0.5f), -util::makeUnitVec3y(), glm::vec2() },
        { glm::vec3(-0.5f,  0.0f, -0.5f), -util::makeUnitVec3y(), glm::vec2() },
        { glm::vec3(-0.5f,  0.0f,  0.5f), -util::makeUnitVec3y(), glm::vec2() },
        { glm::vec3( 0.5f,  0.0f,  0.5f), -util::makeUnitVec3y(), glm::vec2() },
        // Top.
        { glm::vec3( 0.5f,  1.0f,  0.5f),  util::makeUnitVec3y(), glm::vec2() },
        { glm::vec3( 0.5f,  1.0f, -0.5f),  util::makeUnitVec3y(), glm::vec2() },
        { glm::vec3(-0.5f,  1.0f, -0.5f),  util::makeUnitVec3y(), glm::vec2() },
        { glm::vec3(-0.5f,  1.0f, -0.5f),  util::makeUnitVec3y(), glm::vec2() },
        { glm::vec3(-0.5f,  1.0f,  0.5f),  util::makeUnitVec3y(), glm::vec2() },
        { glm::vec3( 0.5f,  1.0f,  0.5f),  util::makeUnitVec3y(), glm::vec2() },
        // Right.
        { glm::vec3( 0.5f,  0.0f,  0.5f),  util::makeUnitVec3x(), glm::vec2() },
        { glm::vec3( 0.5f,  0.0f, -0.5f),  util::makeUnitVec3x(), glm::vec2() },
        { glm::vec3( 0.5f,  1.0f,  0.5f),  util::makeUnitVec3x(), glm::vec2() },
        { glm::vec3( 0.5f,  1.0f,  0.5f),  util::makeUnitVec3x(), glm::vec2() },
        { glm::vec3( 0.5f,  1.0f, -0.5f),  util::makeUnitVec3x(), glm::vec2() },
        { glm::vec3( 0.5f,  0.0f, -0.5f),  util::makeUnitVec3x(), glm::vec2() },
        // Back.
        { glm::vec3( 0.5f,  0.0f, -0.5f), -util::makeUnitVec3z(), glm::vec2() },
        { glm::vec3(-0.5f,  0.0f, -0.5f), -util::makeUnitVec3z(), glm::vec2() },
        { glm::vec3( 0.5f,  1.0f, -0.5f), -util::makeUnitVec3z(), glm::vec2() },
        { glm::vec3( 0.5f,  1.0f, -0.5f), -util::makeUnitVec3z(), glm::vec2() },
        { glm::vec3(-0.5f,  1.0f, -0.5f), -util::makeUnitVec3z(), glm::vec2() },
        { glm::vec3(-0.5f,  0.0f, -0.5f), -util::makeUnitVec3z(), glm::vec2() },
        // Left.
        { glm::vec3(-0.5f,  0.0f,  0.5f), -util::makeUnitVec3x(), glm::vec2() },
        { glm::vec3(-0.5f,  0.0f, -0.5f), -util::makeUnitVec3x(), glm::vec2() },
        { glm::vec3(-0.5f,  1.0f, -0.5f), -util::makeUnitVec3x(), glm::vec2() },
        { glm::vec3(-0.5f,  1.0f, -0.5f), -util::makeUnitVec3x(), glm::vec2() },
        { glm::vec3(-0.5f,  1.0f,  0.5f), -util::makeUnitVec3x(), glm::vec2() },
        { glm::vec3(-0.5f,  0.0f,  0.5f), -util::makeUnitVec3x(), glm::vec2() },
        // Front.
        { glm::vec3(-0.5f,  0.0f,  0.5f),  util::makeUnitVec3z(), glm::vec2() },
        { glm::vec3( 0.5f,  0.0f,  0.5f),  util::makeUnitVec3z(), glm::vec2() },
        { glm::vec3( 0.5f,  1.0f,  0.5f),  util::makeUnitVec3z(), glm::vec2() },
        { glm::vec3( 0.5f,  1.0f,  0.5f),  util::makeUnitVec3z(), glm::vec2() },
        { glm::vec3(-0.5f,  1.0f,  0.5f),  util::makeUnitVec3z(), glm::vec2() },
        { glm::vec3(-0.5f,  0.0f,  0.5f),  util::makeUnitVec3z(), glm::vec2() }
    };

    settings->indexed = false;

    return std::make_unique<Mesh>(vertices, std::vector<uint32_t>(), std::move(settings));
}

//------------------------------------------------------------------------

std::unique_ptr<Mesh> MeshFactory::makePlane(std::unique_ptr<Mesh::Settings> settings)
{
    static const std::vector<Mesh::Vertex> vertices{
        { glm::vec3( 0.5f,  0.0f,  0.5f), util::makeUnitVec3y(), glm::vec2(1.0f, 1.0f) },
        { glm::vec3(-0.5f,  0.0f,  0.5f), util::makeUnitVec3y(), glm::vec2(0.0f, 1.0f) },
        { glm::vec3(-0.5f,  0.0f, -0.5f), util::makeUnitVec3y(), glm::vec2(0.0f, 0.0f) },
        { glm::vec3( 0.5f,  0.0f, -0.5f), util::makeUnitVec3y(), glm::vec2(1.0f, 0.0f) }
    };
    
    static const std::vector<GLuint> indices{
        0, 1, 2, 2, 3, 0
    };

    return std::make_unique<Mesh>(vertices, indices, std::move(settings));
}

//------------------------------------------------------------------------
