#include "Scene.hpp"

#include "Mesh.hpp"
#include "constants.hpp"
#include "util.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include <cstdint>
#include <iostream>
#include <ranges>

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

void Scene::addModelFromFile(const fs::path& path) const noexcept
{
    const auto modelHandle = m_mngr->createModel2();
    Model2* model = m_mngr->get(modelHandle);
    // TODO: Geometry caching.

    Assimp::Importer importer{};
    const aiScene* scene = importer.ReadFile(path.string().c_str(), constants::ASSIMP_LOAD_FLAGS);
    if (scene == nullptr)
    {
        std::cerr << importer.GetErrorString() << "\n";
        // TODO: Get some sort of default geometry instead.
    }

    for (uint32_t i : std::views::iota(0u, scene->mNumMeshes))
    {
        const aiMesh* mesh = scene->mMeshes[i];
        model->addMesh(m_mngr->createMesh(Mesh::Geometry{
            .vertices = std::span(vertexBuffer()->getWritePtr<Vertex>(), mesh->mNumVertices),
            .indices = std::span(indexBuffer()->getWritePtr<GLuint>(), mesh->mNumFaces * 3)
        }));

        for (uint32_t j : std::views::iota(0u, mesh->mNumVertices))
        {
            const Vertex vertex{
                .pos = util::vec3FromAiVector3D(mesh->mVertices[j]),
                .nrm = util::vec3FromAiVector3D(mesh->mNormals[j]),
                .uv = mesh->HasTextureCoords(0) ? util::vec2FromAiVector3D(mesh->mTextureCoords[0][j]) : glm::vec2()
            };
            vertexBuffer()->pushData<Vertex>(&vertex);
        }

        for (uint32_t j : std::views::iota(0u, mesh->mNumFaces))
        {
            const aiFace& face = mesh->mFaces[j];
            indexBuffer()->pushData<GLuint>(face.mIndices, face.mNumIndices);
        }
    }

    for (uint32_t i : std::views::iota(0u, scene->mNumMaterials))
    {
        const aiMaterial* material = scene->mMaterials[i];

        if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
        {
            aiString tempMaterialPath;
            if (material->GetTexture(aiTextureType_DIFFUSE, 0, &tempMaterialPath) == AI_FAILURE) continue;
            fs::path materialPath = path.parent_path() / tempMaterialPath.data;
            m_textures.push_back(Texture::fromFile(m_mngr, materialPath));
        }
    }
}

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
