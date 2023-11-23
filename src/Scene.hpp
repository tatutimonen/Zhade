#pragma once

#include "Buffer.hpp"
#include "Handle.hpp"
#include "Model.hpp"
#include "ResourceManager.hpp"
#include "Texture.hpp"
#include "common.hpp"

#include <assimp/scene.h>
#include <robin_hood.h>

#include <atomic>
#include <string>
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
        const Model* model) const noexcept;
    [[nodiscard]] VerticesLoadInfo loadVertices(const aiMesh* aiMeshPtr) const noexcept;
    [[nodiscard]] IndicesLoadInfo loadIndices(const aiMesh* aiMeshPtr) const noexcept;
    [[nodiscard]] Handle<Texture> loadTexture(const aiMaterial* aiMaterialPtr, aiTextureType textureType,
        const fs::path& dir) const noexcept;

    [[nodiscard]] const Buffer* vertexBuffer() const noexcept { return m_mngr->get(m_vertexBuffer); }
    [[nodiscard]] const Buffer* indexBuffer() const noexcept { return m_mngr->get(m_indexBuffer); }
    [[nodiscard]] const Buffer* meshBuffer() const noexcept { return m_mngr->get(m_meshBuffer); }

    static inline std::atomic_uint32_t s_modelIdCounter = 0;

    ResourceManager* m_mngr;
    Handle<Buffer> m_vertexBuffer;
    Handle<Buffer> m_indexBuffer;
    Handle<Buffer> m_meshBuffer;
    Handle<Texture> m_defaultTexture;
    mutable std::vector<Handle<Model>> m_models;
    mutable robin_hood::unordered_map<fs::path, Handle<Model>> m_modelCache;

    friend class Renderer;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
