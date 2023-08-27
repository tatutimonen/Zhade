#include "Scene.hpp"

#include "Mesh.hpp"
#include "util.hpp"

#include <assimp/Importer.hpp>

#include <array>
#include <cstdint>
#include <format>
#include <future>
#include <iostream>
#include <ranges>
#include <span>

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

Scene::Scene(SceneDescriptor desc)
    : m_mngr{desc.mngr}
{
    m_vertexBuffer = m_mngr->createBuffer(desc.vertexBufferDesc);
    m_indexBuffer = m_mngr->createBuffer(desc.vertexBufferDesc);
    m_defaultTexture = Texture::makeDefault(m_mngr);
}

//------------------------------------------------------------------------

Scene::~Scene()
{
    m_mngr->destroy(m_vertexBuffer);
    m_mngr->destroy(m_vertexBuffer);
    m_mngr->destroy(m_defaultTexture);
}

//------------------------------------------------------------------------

void Scene::addModelFromFile(const fs::path& path) const noexcept
{
    const auto modelHandle = m_mngr->createModel2();
    Model2* model = m_mngr->get(modelHandle);

    Assimp::Importer importer{};
    const aiScene* scene = importer.ReadFile(path.string().c_str(), ASSIMP_LOAD_FLAGS);

    for (const aiMesh* mesh : std::span(scene->mMeshes, scene->mNumMeshes))
    {
        const auto desc = loadMeshConstituentsAsync(scene, mesh, SUPPORTED_TEXTURE_TYPES, path.parent_path());
        const auto meshHandle = m_mngr->createMesh(desc);
        model->addMesh(meshHandle);
    }

    m_models.push_back(modelHandle);
}

//------------------------------------------------------------------------

MeshDescriptor Scene::loadMeshConstituentsAsync(const aiScene* scene, const aiMesh* mesh,
    const std::bitset<AI_TEXTURE_TYPE_MAX>& textureTypes, const fs::path& basePath) const noexcept
{
    auto verticesFuture = std::async(
        std::launch::async,
        [this](auto&& mesh) { return loadVertices(std::forward<decltype(mesh)>(mesh)); },
        mesh
    );
    auto indicesFuture = std::async(
        std::launch::async,
        [this](auto&& mesh) { return loadIndices(std::forward<decltype(mesh)>(mesh)); },
        mesh
    );

    std::array<std::future<Handle<Texture>>, AI_TEXTURE_TYPE_MAX> textureFutures;
    for (uint32_t textureType = aiTextureType_NONE; textureType < AI_TEXTURE_TYPE_MAX; ++textureType)
    {
        if (textureTypes.test(textureType) == false) continue;
        textureFutures[textureType] = std::async(
            std::launch::async,
            [this](auto&&... args) { return loadTexture(std::forward<decltype(args)>(args)...); },
            scene, mesh, static_cast<aiTextureType>(textureType), basePath
        );
    }

    return MeshDescriptor{
        .vertices = verticesFuture.get(),
        .indices = indicesFuture.get(),
        .diffuse = textureFutures[aiTextureType_DIFFUSE].get()
    };
}

//------------------------------------------------------------------------

std::span<Vertex> Scene::loadVertices(const aiMesh* mesh) const noexcept
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

    return std::span(verticesStart, mesh->mNumVertices);
}

//------------------------------------------------------------------------

std::span<GLuint> Scene::loadIndices(const aiMesh* mesh) const noexcept
{
    GLuint* indicesStart = indexBuffer()->getWritePtr<GLuint>();

    for (const aiFace& face : std::span(mesh->mFaces, mesh->mNumFaces))
    {
        indexBuffer()->pushData<GLuint>(face.mIndices, face.mNumIndices);
    }

    return std::span(indicesStart, mesh->mNumFaces * 3);
}

//------------------------------------------------------------------------

Handle<Texture> Scene::loadTexture(const aiScene* scene, const aiMesh* mesh,
    aiTextureType textureType, const fs::path& basePath) const noexcept
{
    const aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

    if (material->GetTextureCount(textureType) == 0) return m_defaultTexture;

    aiString tempMaterialPath;
    material->GetTexture(textureType, 0, &tempMaterialPath);
    const fs::path materialPath = basePath / tempMaterialPath.data;
    return Texture::fromFile(m_mngr, materialPath);
}

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
