#include "Scene.hpp"

#include "constants.hpp"
#include "util.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include <iostream>

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

Scene::addNamedModel(std::string_view filename) const noexcept
{
    const auto modelHandle = m_mngr->createModel();
    auto model = mngr->get(modelHandle);
    // TODO: Geometry caching.

    Assimp::Importer importer{};
    const aiScene* scene = importer.ReadFile(filename, constants::ASSIMP_LOAD_FLAGS);
    if (scene == nullptr)
    {
        std::cerr << importer.GetErrorString() << "\n";
        // TODO: Get some sort of default geometry instead.
    }

    for (int i = 0; i < scene->mNumMeshes; ++i)
    {
        const aiMesh* mesh = scene->mMeshes[i];
        const Buffer* vertexBuffer = m_mngr->get(m_vertexBuffer);
        for (int j = 0; j < mesh->mNumVertices; ++j)
        {
            Vertex vtx{
                .pos = util::vec3FromAiVector3D(mesh->mVertices[j]),
                .nrm = util::vec3FromAiVector3D(mesh->mNormals[j]),
                .uv = mesh->HasTextureCoords(0) ? mesh->mTextureCoords[0][j] : glm::vec2()
            };
            vertexBuffer->
        }
    }
}

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
