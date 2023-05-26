#include "Scene.hpp"

#include "Mesh.hpp"
#include "constants.hpp"
#include "util.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include <cstdint>
#include <iostream>
#include <ranges>
#include <span>

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

void Scene::addModelFromFile(const fs::path& path) const noexcept
{
    const std::string modelName = path.stem().string();
    if (m_modelCache.contains(modelName))
    {
        m_models.push_back(m_modelCache.at(modelName));
        return;
    }

    const auto modelHandle = m_mngr->createModel2();
    Model2* model = m_mngr->get(modelHandle);

    Assimp::Importer importer{};
    const aiScene* scene = importer.ReadFile(path.string().c_str(), constants::ASSIMP_LOAD_FLAGS);
    if (scene == nullptr)
    {
        std::cerr << importer.GetErrorString() << "\n";
        // TODO: Get some sort of default geometry instead.
        m_mngr->destroy(modelHandle);
        return;
    }

    for (const aiMesh* mesh : std::span(scene->mMeshes, scene->mNumMeshes))
    {
        Vertex* verticesStart = vertexBuffer()->getWritePtr<Vertex>();
        for (uint32_t idx : std::views::iota(0u, mesh->mNumVertices))
        {
            const Vertex vertex{
                .pos = util::vec3FromAiVector3D(mesh->mVertices[idx]),
                .nrm = util::vec3FromAiVector3D(mesh->mNormals[idx]),
                .uv = mesh->HasTextureCoords(0) ? util::vec2FromAiVector3D(mesh->mTextureCoords[0][idx]) : glm::vec2{}
            };
            vertexBuffer()->pushData<Vertex>(&vertex);
        }

        GLuint* indicesStart = indexBuffer()->getWritePtr<GLuint>();
        for (const aiFace& face : std::span(mesh->mFaces, mesh->mNumFaces))
        {
            indexBuffer()->pushData<GLuint>(face.mIndices, face.mNumIndices);
        }

        const aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
        {
            aiString tempMaterialPath;
            material->GetTexture(aiTextureType_DIFFUSE, 0, &tempMaterialPath);
            const fs::path materialPath = path.parent_path() / tempMaterialPath.data;
            const std::string materialPathStr = materialPath.string();
            m_textures.push_back(Texture::fromFile(m_mngr, materialPath));
        }

        const auto meshHandle = m_mngr->createMesh(Mesh::Desc{
            .geometry = {
                .vertices = std::span(verticesStart, mesh->mNumVertices),
                .indices = std::span(indicesStart, mesh->mNumFaces * 3)
            },
            .materialID = 0,
            .diffuseID = m_baseTexture++
        });
        model->addMesh(meshHandle);
    }

    model->m_ID = m_modelID++;
    m_modelCache.insert({ modelName, modelHandle });
    m_models.push_back(modelHandle);
}

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
