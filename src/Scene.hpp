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
#include <filesystem>
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

    [[nodiscard]] std::span<Handle<Model>> getModels() const noexcept { return m_models; }

    void addModelFromFile(const fs::path& path) const noexcept;
    [[nodiscard]] const Buffer* vertexBuffer() const noexcept { return m_mngr->get(m_vertexBuffer); }
    [[nodiscard]] const Buffer* indexBuffer() const noexcept { return m_mngr->get(m_indexBuffer); }

private:
    template<typename T>
    requires std::same_as<T, Vertex> or std::same_as<T, GLuint>
    struct LoadInfo
    {
        GLuint base;
        std::span<T> span;
    };

    [[nodiscard]] Handle<Mesh> loadMesh(const aiScene* scene, const aiMesh* mesh, const fs::path& path,
        const Handle<Model>& model) const noexcept;
    [[nodiscard]] LoadInfo<Vertex> loadVertices(const aiMesh* mesh) const noexcept;
    [[nodiscard]] LoadInfo<GLuint> loadIndices(const aiMesh* mesh) const noexcept;
    [[nodiscard]] Handle<Texture> loadTexture(const aiScene* scene, const aiMesh* mesh, aiTextureType textureType,
        const fs::path& dir) const noexcept;

    static inline std::atomic_uint32_t s_meshIdCounter = 0;

    ResourceManager* m_mngr;
    Handle<Buffer> m_vertexBuffer;
    Handle<Buffer> m_indexBuffer;
    Handle<Texture> m_defaultTexture;
    mutable std::vector<Handle<Model>> m_models;
    mutable robin_hood::unordered_map<fs::path, Handle<Model>> m_modelCache;

    friend class Renderer;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
