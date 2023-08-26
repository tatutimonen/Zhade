#include "Scene.hpp"

#include "Mesh.hpp"
#include "util.hpp"

#include <assimp/Importer.hpp>

#include <cstdint>
#include <format>
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
    m_buffers = {
        .vertex = m_mngr->createBuffer(desc.vertexBufferDesc),
        .index = m_mngr->createBuffer(desc.vertexBufferDesc)
    };
    m_defaultTexture = Texture::makeDefault(m_mngr);
}

//------------------------------------------------------------------------

Scene::~Scene()
{
    m_mngr->destroy(m_buffers.vertex);
    m_mngr->destroy(m_buffers.index);
    m_mngr->destroy(m_defaultTexture);
}

//------------------------------------------------------------------------

void Scene::addModelFromFile(const fs::path& path) const noexcept
{
    const auto modelHandle = m_mngr->createModel2();
    Model2* model = m_mngr->get(modelHandle);

    static Assimp::Importer importer{};
    const aiScene* scene = importer.ReadFile(path.string().c_str(), ASSIMP_LOAD_FLAGS);

    for (const aiMesh* mesh : std::span(scene->mMeshes, scene->mNumMeshes))
    {
        model->addMesh(
            m_mngr->createMesh({
                .vertices = loadVertices(mesh),
                .indices = loadIndices(mesh),
                .diffuse = loadTexture(scene, mesh, aiTextureType_DIFFUSE, path)
            })
        );
    }

    m_models.push_back(modelHandle);
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

Handle<Texture> Scene::loadTexture(const aiScene* scene, const aiMesh* mesh, aiTextureType textureType,
    const fs::path& modelPath) const noexcept
{
    const aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

    if (material->GetTextureCount(textureType) == 0) return m_defaultTexture;

    aiString tempMaterialPath;
    material->GetTexture(textureType, 0, &tempMaterialPath);
    const fs::path materialPath = modelPath.parent_path() / tempMaterialPath.data;
    return Texture::fromFile(m_mngr, materialPath);
}

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
