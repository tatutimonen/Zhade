#pragma once

#include "Buffer.hpp"
#include "DirectionalLight.hpp"
#include "Handle.hpp"
#include "Model.hpp"
#include "ResourceManager.hpp"
#include "Texture.hpp"

#include <assimp/scene.h>
#include <robin_hood.h>

#include <vector>

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

struct SceneDescriptor
{
    ResourceManager* mngr;
    BufferDescriptor vertexBufferDesc{
        .byteSize = GIB_BYTES/2,
        .usage = BufferUsage::VERTEX
    };
    BufferDescriptor indexBufferDesc{
        .byteSize = GIB_BYTES/2,
        .usage = BufferUsage::INDEX
    };
    BufferDescriptor meshBufferDesc{
        .byteSize = GIB_BYTES/4,
        .usage = BufferUsage::STORAGE,
        .indexedBindings = {
            {.target = BufferUsage::STORAGE, .index = MESH_BINDING}
        }
    };
    DirectionalLightDescriptor sunLightDesc;
};

//------------------------------------------------------------------------

class Scene
{
public:
    explicit Scene(SceneDescriptor desc);
    ~Scene();

    Scene(const Scene&) = delete;
    Scene& operator=(const Scene&) = delete;
    Scene(Scene&&) = delete;
    Scene& operator=(Scene&&) = delete;

    [[nodiscard]] const DirectionalLight& sun() { return m_sunLight; }
    [[nodiscard]] std::span<Handle<Model>> models() { return m_models; }

    void addModelFromFile(const fs::path& path);

private:
    struct VerticesLoadInfo { GLuint base; };
    struct IndicesLoadInfo { GLuint base; GLuint extent; };

    [[nodiscard]] Mesh loadMesh(const aiScene* aiScenePtr, const aiMesh* aiMeshPtr, const fs::path& path,
        Model* model);
    [[nodiscard]] VerticesLoadInfo loadVertices(const aiMesh* aiMeshPtr);
    [[nodiscard]] IndicesLoadInfo loadIndices(const aiMesh* aiMeshPtr);
    [[nodiscard]] Handle<Texture> loadTexture(const aiMaterial* aiMaterialPtr, aiTextureType textureType,
        const fs::path& dir);

    [[nodiscard]] Buffer* buffer(const Handle<Buffer>& handle) { return m_mngr->get(handle); }

    ResourceManager* m_mngr;
    Handle<Buffer> m_vertexBuffer;
    Handle<Buffer> m_indexBuffer;
    Handle<Buffer> m_meshBuffer;
    DirectionalLight m_sunLight;
    Handle<Texture> m_defaultTexture;
    std::vector<Handle<Model>> m_models;
    robin_hood::unordered_map<fs::path, Handle<Model>> m_modelCache;

    friend class Renderer;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
