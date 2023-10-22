#include "Scene.hpp"

#include "Mesh.hpp"
#include "util.hpp"

#include <assimp/Importer.hpp>

#include <array>
#include <cstdint>
#include <future>
#include <memory_resource>
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
    m_indexBuffer = m_mngr->createBuffer(desc.indexBufferDesc);
    m_defaultTexture = Texture::makeDefault(m_mngr);
}

//------------------------------------------------------------------------

Scene::~Scene()
{
    m_mngr->destroy(m_vertexBuffer);
    m_mngr->destroy(m_indexBuffer);
    m_mngr->destroy(m_defaultTexture);
}

//------------------------------------------------------------------------

void Scene::addModelFromFile(const fs::path& path) const noexcept
{
    if (m_modelCache.contains(path) and m_mngr->get(m_modelCache[path]) != nullptr)
    {
        m_models.push_back(m_modelCache[path]);
        return;
    }

    Assimp::Importer importer{};
    const aiScene* scene = importer.ReadFile(path.string().c_str(), ASSIMP_LOAD_FLAGS);

    const auto model = m_mngr->createModel({});

    std::vector<Handle<Mesh>> meshes;
    for (const aiMesh* mesh : std::span(scene->mMeshes, scene->mNumMeshes))
    {
        meshes.push_back(loadMesh(scene, mesh, path, model));
    }

    m_mngr->get(model)->appendMeshes(meshes);
    m_models.push_back(model);
    m_modelCache[path] = model;
}

//------------------------------------------------------------------------

Handle<Mesh> Scene::loadMesh(const aiScene* scene, const aiMesh* mesh, const fs::path& path,
    const Handle<Model>& model) const noexcept
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

    const auto verticesLoadInfo = verticesFuture.get();
    const auto indicesLoadInfo = indicesFuture.get();

    auto meshHandle = m_mngr->createMesh({
        .vertices = verticesLoadInfo.span,
        .indices = indicesLoadInfo.span,
        .baseVertex = verticesLoadInfo.base,
        .firstIndex = indicesLoadInfo.base,
        .diffuse = loadTexture(scene, mesh, aiTextureType_DIFFUSE, path.parent_path()),
        .model = model,
        .id = s_meshIdCounter++
    });
    return meshHandle;
}

//------------------------------------------------------------------------

Scene::LoadInfo<Vertex> Scene::loadVertices(const aiMesh* mesh) const noexcept
{
    static std::array<uint8_t, KIB_BYTES*2> buf;
    std::pmr::monotonic_buffer_resource rsrc{buf.data(), buf.size()};
    std::pmr::vector<Vertex> vertices{&rsrc};

    for (uint32_t idx : stdv::iota(0u, mesh->mNumVertices))
    {
        vertices.emplace_back(
            util::vec3FromAiVector3D(mesh->mVertices[idx]),
            util::vec3FromAiVector3D(mesh->mNormals[idx]),
            mesh->HasTextureCoords(0) ? util::vec2FromAiVector3D(mesh->mTextureCoords[0][idx]) : glm::vec2{}
        );
    }

    Vertex* verticesStart = vertexBuffer()->getWritePtr<Vertex>();
    vertexBuffer()->pushData(vertices.data(), vertices.size());

    return {
        .base = implicit_cast<GLuint>(verticesStart - vertexBuffer()->getPtr<Vertex>()),
        .span = std::span(verticesStart, mesh->mNumVertices)
    };
}

//------------------------------------------------------------------------

Scene::LoadInfo<GLuint> Scene::loadIndices(const aiMesh* mesh) const noexcept
{
    static std::array<uint8_t, KIB_BYTES> buf;
    std::pmr::monotonic_buffer_resource rsrc{buf.data(), buf.size()};
    std::pmr::vector<GLuint> indices{&rsrc};

    for (const aiFace& face : std::span(mesh->mFaces, mesh->mNumFaces))
    {
        indices.append_range(std::span(face.mIndices, face.mNumIndices));
    }

    GLuint* indicesStart = indexBuffer()->getWritePtr<GLuint>();
    indexBuffer()->pushData(indices.data(), indices.size());

    return {
        .base = implicit_cast<GLuint>(indicesStart - indexBuffer()->getPtr<GLuint>()),
        .span = std::span(indicesStart, mesh->mNumFaces * 3)
    };
}

//------------------------------------------------------------------------

Handle<Texture> Scene::loadTexture(const aiScene* scene, const aiMesh* mesh, aiTextureType textureType,
    const fs::path& dir) const noexcept
{
    const aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

    if (material->GetTextureCount(textureType) == 0)
    {
        return m_defaultTexture;
    }

    aiString tempMaterialPath;
    material->GetTexture(textureType, 0, &tempMaterialPath);
    const fs::path materialPath = dir / tempMaterialPath.data;

    return Texture::fromFile(m_mngr, materialPath);
}

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
