#pragma once

#include "Mesh.hpp"
#include "ShaderProgram.hpp"
#include "Texture2D.hpp"

#include <memory>

//------------------------------------------------------------------------

class MeshFactory {
public:
    MeshFactory() = default;

    static std::unique_ptr<Mesh> makeCube(
        std::unique_ptr<Mesh::Settings> settings = std::make_unique<Mesh::Settings>());

    static std::unique_ptr<Mesh> makePlane(
        std::unique_ptr<Mesh::Settings> settings = std::make_unique<Mesh::Settings>());
};

//------------------------------------------------------------------------
