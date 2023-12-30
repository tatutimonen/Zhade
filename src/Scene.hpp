#pragma once

#include "Buffer.hpp"
#include "DirectionalLight.hpp"
#include "Handle.hpp"
#include "Model.hpp"
#include "ResourceManager.hpp"
#include "Texture.hpp"
#include "common.hpp"

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
    BufferDescriptor vertexBufferDesc{.byteSize = GIB_BYTES/2, .usage = BufferUsage::VERTEX};
    BufferDescriptor indexBufferDesc{.byteSize = GIB_BYTES/2, .usage = BufferUsage::INDEX};
    BufferDescriptor meshBufferDesc{.byteSize = GIB_BYTES/4, .usage = BufferUsage::STORAGE};
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

    [[nodiscard]] std::span<Handle<Model>> models() const noexcept { return m_models; }

    void addModelFromFile(const fs::path& path) const noexcept;

private:
    struct VerticesLoadInfo
    {
        GLuint base;
    };

    struct IndicesLoadInfo
    {
        GLuint base;
        GLuint extent;
    };

    [[nodiscard]] Mesh loadMesh(const aiScene* aiScenePtr, const aiMesh* aiMeshPtr, const fs::path& path,
        Model* model) const noexcept;
    [[nodiscard]] VerticesLoadInfo loadVertices(const aiMesh* aiMeshPtr) const noexcept;
    [[nodiscard]] IndicesLoadInfo loadIndices(const aiMesh* aiMeshPtr) const noexcept;
    [[nodiscard]] Handle<Texture> loadTexture(const aiMaterial* aiMaterialPtr, aiTextureType textureType,
        const fs::path& dir) const noexcept;

    [[nodiscard]] const Buffer* buffer(const Handle<Buffer>& handle) const noexcept { return m_mngr->get(handle); }

    Handle<Buffer> m_vertexBuffer;
    Handle<Buffer> m_indexBuffer;
    Handle<Buffer> m_meshBuffer;
    DirectionalLight m_sunLight;
    Handle<Texture> m_defaultTexture;
    mutable std::vector<Handle<Model>> m_models;
    mutable robin_hood::unordered_map<fs::path, Handle<Model>> m_modelCache;
    ResourceManager* m_mngr;

    friend class Renderer;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
