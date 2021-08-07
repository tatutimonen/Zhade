#include "MeshFactory.hpp"

//------------------------------------------------------------------------

std::unique_ptr<Mesh> MeshFactory::makeCube(std::unique_ptr<Mesh::Settings> settings)
{
    const std::vector<Mesh::Vertex> vertices{
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

    return std::make_unique<Mesh>(vertices, indices, std::move(settings));
}

//------------------------------------------------------------------------

std::unique_ptr<Mesh> MeshFactory::makePlane(std::unique_ptr<Mesh::Settings> settings)
{
    const std::vector<Mesh::Vertex> vertices{
        { glm::vec3( 0.5f,  0.0f,  0.5f), Util::makeUnitVec3y(), glm::vec2(1.0f, 1.0f) },
        { glm::vec3(-0.5f,  0.0f,  0.5f), Util::makeUnitVec3y(), glm::vec2(0.0f, 1.0f) },
        { glm::vec3(-0.5f,  0.0f, -0.5f), Util::makeUnitVec3y(), glm::vec2(0.0f, 0.0f) },
        { glm::vec3( 0.5f,  0.0f, -0.5f), Util::makeUnitVec3y(), glm::vec2(1.0f, 0.0f) }
    };
    
    const std::vector<GLuint> indices{
        0, 1, 2, 2, 3, 0
    };

    return std::make_unique<Mesh>(vertices, indices, std::move(settings));
}

//------------------------------------------------------------------------
