#include "Scene.hpp"

#include "common.hpp"
#include "util.hpp"

#include <assimp/Importer.hpp>

#include <array>
#include <future>
#include <memory_resource>
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
    m_meshBuffer = m_mngr->createBuffer(desc.meshBufferDesc);
    m_defaultTexture = Texture::makeDefault(m_mngr);
}

//------------------------------------------------------------------------

Scene::~Scene()
{
    m_mngr->destroy(m_vertexBuffer);
    m_mngr->destroy(m_indexBuffer);
    m_mngr->destroy(m_meshBuffer);
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
    const aiScene* aiScenePtr = importer.ReadFile(path.string().c_str(), ASSIMP_LOAD_FLAGS);

    const Handle<Model> model = m_mngr->createModel({
        .id = s_modelIdCounter++,
        .mngr = m_mngr
    });
    const Model* modelPtr = m_mngr->get(model);

    Mesh* meshesStart = meshBuffer()->writePtr<Mesh>();
    for (const aiMesh* aiMeshPtr : std::span(aiScenePtr->mMeshes, aiScenePtr->mNumMeshes))
    {
        const Mesh mesh = loadMesh(aiScenePtr, aiMeshPtr, path, modelPtr);
        meshBuffer()->pushData(&mesh);
    }
    modelPtr->m_meshes = std::span(meshesStart, aiScenePtr->mNumMeshes);

    m_models.push_back(model);
    m_modelCache[path] = model;
}

//------------------------------------------------------------------------

Mesh Scene::loadMesh(const aiScene* aiScenePtr, const aiMesh* aiMeshPtr, const fs::path& path,
    const Model* modelPtr) const noexcept
{
    auto verticesFuture = std::async(
        std::launch::async,
        [this](auto&& mesh) { return loadVertices(std::forward<decltype(mesh)>(mesh)); },
        aiMeshPtr
    );
    auto indicesFuture = std::async(
        std::launch::async,
        [this](auto&& mesh) { return loadIndices(std::forward<decltype(mesh)>(mesh)); },
        aiMeshPtr
    );
    auto diffuse = loadTexture(
        aiScenePtr->mMaterials[aiMeshPtr->mMaterialIndex],
        aiTextureType_DIFFUSE,
        path.parent_path()
    );
    modelPtr->m_textures.push_back(diffuse);

    const auto verticesLoadInfo = verticesFuture.get();
    const auto indicesLoadInfo = indicesFuture.get();

    return {
        .numIndices = indicesLoadInfo.extent,
        .firstIndex = indicesLoadInfo.base,
        .baseVertex = verticesLoadInfo.base,
        .transformation = modelPtr->m_transformation,
        .textures = {
            .diffuse = m_mngr->get(diffuse)->handle()
        },
        .alive = true
    };
}

//------------------------------------------------------------------------

Scene::VerticesLoadInfo Scene::loadVertices(const aiMesh* aiMeshPtr) const noexcept
{
    static std::array<uint8_t, KIB_BYTES*2> buf;
    std::pmr::monotonic_buffer_resource rsrc{buf.data(), buf.size()};
    std::pmr::vector<Vertex> vertices{&rsrc};

    for (uint32_t idx : stdv::iota(0u, aiMeshPtr->mNumVertices))
    {
        vertices.emplace_back(
            util::vec3FromAiVector3D(aiMeshPtr->mVertices[idx]),
            util::vec3FromAiVector3D(aiMeshPtr->mNormals[idx]),
            aiMeshPtr->HasTextureCoords(0) ? util::vec2FromAiVector3D(aiMeshPtr->mTextureCoords[0][idx])
                                           : glm::vec2{}
        );
    }

    Vertex* verticesStart = vertexBuffer()->writePtr<Vertex>();
    vertexBuffer()->pushData(vertices.data(), vertices.size());

    return {
        .base = implicit_cast<GLuint>(verticesStart - vertexBuffer()->ptr<Vertex>())
    };
}

//------------------------------------------------------------------------

Scene::IndicesLoadInfo Scene::loadIndices(const aiMesh* aiMeshPtr) const noexcept
{
    static std::array<uint8_t, KIB_BYTES> buf;
    std::pmr::monotonic_buffer_resource rsrc{buf.data(), buf.size()};
    std::pmr::vector<GLuint> indices{&rsrc};

    for (const aiFace& face : std::span(aiMeshPtr->mFaces, aiMeshPtr->mNumFaces))
    {
        indices.append_range(std::span(face.mIndices, face.mNumIndices));
    }

    GLuint* indicesStart = indexBuffer()->writePtr<GLuint>();
    indexBuffer()->pushData(indices.data(), indices.size());

    return {
        .base = implicit_cast<GLuint>(indicesStart - indexBuffer()->ptr<GLuint>()),
        .extent = aiMeshPtr->mNumFaces * 3
    };
}

//------------------------------------------------------------------------

Handle<Texture> Scene::loadTexture(const aiMaterial* aiMaterialPtr, aiTextureType textureType, const fs::path& dir)
    const noexcept
{
    if (aiMaterialPtr->GetTextureCount(textureType) == 0)
    {
        return m_defaultTexture;
    }

    aiString tempMaterialPath;
    aiMaterialPtr->GetTexture(textureType, 0, &tempMaterialPath);
    const fs::path materialPath = dir / tempMaterialPath.C_Str();

    return Texture::fromFile(m_mngr, materialPath);
}

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
