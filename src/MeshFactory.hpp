#pragma once

#include "Mesh.hpp"
#include "ShaderProgram.hpp"

#include <memory>

//------------------------------------------------------------------------

class MeshFactory {
public:
    MeshFactory() = delete;

    static std::unique_ptr<Mesh> makeCube(
        const Mesh::Settings& settings = Mesh::Settings());

    static std::unique_ptr<Mesh> makePlane(
        const Mesh::Settings& settings = Mesh::Settings());
};

//------------------------------------------------------------------------
