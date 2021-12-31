#pragma once

#include "Mesh.hpp"
#include "ShaderProgram.hpp"

#include <memory>
#include <memory_resource>

//------------------------------------------------------------------------

class MeshFactory {
public:
    MeshFactory() = delete;

    static std::unique_ptr<Mesh> makeCube(
        std::unique_ptr<Mesh::Settings> settings = std::make_unique<Mesh::Settings>());

    static std::unique_ptr<Mesh> makePlane(
        std::unique_ptr<Mesh::Settings> settings = std::make_unique<Mesh::Settings>());
};

//------------------------------------------------------------------------
