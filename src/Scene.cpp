#include "Scene.hpp"

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
    : m_sunLight{desc.sunLightDesc},
      m_mngr{desc.mngr}
{
    m_vertexBuffer = m_mngr->createBuffer(desc.vertexBufferDesc);
    m_indexBuffer = m_mngr->createBuffer(desc.indexBufferDesc);
    m_meshBuffer = m_mngr->createBuffer(desc.meshBufferDesc);
    m_defaultTexture = Texture::makeDefault(m_mngr);
}

//------------------------------------------------------------------------

Scene::~Scene()
{
    for (const Handle<Model>& modelHandle : m_models) {
        m_mngr->destroy(modelHandle);
    }
    m_mngr->destroy(m_vertexBuffer);
    m_mngr->destroy(m_indexBuffer);
    m_mngr->destroy(m_meshBuffer);
    m_mngr->destroy(m_defaultTexture);
}

//------------------------------------------------------------------------

void Scene::addModelFromFile(const fs::path& path)
{
    if (m_modelCache.contains(path) and m_mngr->exists(m_modelCache[path])) {
        const Handle<Model>& modelHandle = m_modelCache[path];
        m_models.push_back(modelHandle);
        for (auto& mesh : m_mngr->get(modelHandle)->m_meshes) {
            ++mesh.refCount;
        }
        return;
    }

    Assimp::Importer importer{};
    const aiScene* aiScenePtr = importer.ReadFile(path.string().c_str(), ASSIMP_LOAD_FLAGS);

    const Handle<Model> model = m_mngr->createModel({.mngr = m_mngr});
    Model* modelPtr = m_mngr->get(model);

    Mesh* meshesStart = buffer(m_meshBuffer)->writePtr<Mesh>();
    for (const aiMesh* aiMeshPtr : std::span{aiScenePtr->mMeshes, aiScenePtr->mNumMeshes}) {
        Mesh mesh = loadMesh(aiScenePtr, aiMeshPtr, path, modelPtr);
        buffer(m_meshBuffer)->pushData(&mesh);
    }
    modelPtr->m_meshes = std::span{meshesStart, aiScenePtr->mNumMeshes};

    m_models.push_back(model);
    m_modelCache[path] = model;
}

//------------------------------------------------------------------------

Mesh Scene::loadMesh(const aiScene* aiScenePtr, const aiMesh* aiMeshPtr, const fs::path& path,
    Model* modelPtr)
{
    auto verticesFuture = std::async(
        std::launch::async,
        [&] { return loadVertices(std::forward<decltype(aiMeshPtr)>(aiMeshPtr)); }
    );
    auto indicesFuture = std::async(
        std::launch::async,
        [&] { return loadIndices(std::forward<decltype(aiMeshPtr)>(aiMeshPtr)); }
    );
    const Handle<Texture> diffuse = loadTexture(
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
        .modelMatT = glm::transpose(modelPtr->m_mat),
        .normalMat = glm::transpose(glm::inverse(modelPtr->m_mat)),
        .textures = {
            .diffuse = m_mngr->get(diffuse)->handle()
        }
    };
}

//------------------------------------------------------------------------

Scene::VerticesLoadInfo Scene::loadVertices(const aiMesh* aiMeshPtr)
{
    std::array<uint8_t, KIB_BYTES * 3> buf;
    std::pmr::monotonic_buffer_resource rsrc{buf.data(), buf.size()};
    std::pmr::vector<Vertex> vertices{&rsrc};

    for (uint32_t idx : stdv::iota(0u, aiMeshPtr->mNumVertices)) {
        vertices.emplace_back(
            util::vec3FromAiVector3D(aiMeshPtr->mVertices[idx]),
            util::vec3FromAiVector3D(aiMeshPtr->mNormals[idx]),
            aiMeshPtr->HasTextureCoords(0) ? util::vec2FromAiVector3D(aiMeshPtr->mTextureCoords[0][idx])
                                           : glm::vec2{}
        );
    }

    Vertex* verticesStart = buffer(m_vertexBuffer)->writePtr<Vertex>();
    buffer(m_vertexBuffer)->pushData(vertices.data(), vertices.size());

    return {
        .base = implicit_cast<GLuint>(verticesStart - buffer(m_vertexBuffer)->ptr<Vertex>())
    };
}

//------------------------------------------------------------------------

Scene::IndicesLoadInfo Scene::loadIndices(const aiMesh* aiMeshPtr)
{
    std::array<uint8_t, KIB_BYTES> buf;
    std::pmr::monotonic_buffer_resource rsrc{buf.data(), buf.size()};
    std::pmr::vector<GLuint> indices{&rsrc};

    for (const aiFace& face : std::span{aiMeshPtr->mFaces, aiMeshPtr->mNumFaces}) {
        for (auto idx : std::span{face.mIndices, face.mNumIndices}) {
            indices.push_back(idx);
        }
    }

    GLuint* indicesStart = buffer(m_indexBuffer)->writePtr<GLuint>();
    buffer(m_indexBuffer)->pushData(indices.data(), indices.size());

    return {
        .base = implicit_cast<GLuint>(indicesStart - buffer(m_indexBuffer)->ptr<GLuint>()),
        .extent = aiMeshPtr->mNumFaces * 3
    };
}

//------------------------------------------------------------------------

Handle<Texture> Scene::loadTexture(const aiMaterial* aiMaterialPtr, aiTextureType textureType, const fs::path& dir)
   
{
    if (aiMaterialPtr->GetTextureCount(textureType) == 0) {
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
