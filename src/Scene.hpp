#pragma once

#include "Buffer.hpp"
#include "Handle.hpp"
#include "Model.hpp"
#include "ResourceManager.hpp"
#include "Texture.hpp"
#include "common.hpp"

#include <assimp/scene.h>

#include <filesystem>
#include <string>
#include <vector>

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

struct SceneBuffers
{
    Handle<Buffer> vertex;
    Handle<Buffer> index;
};
 
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

    [[nodiscard]] std::span<Handle<Model2>> getModels() const noexcept { return m_models; }

    void addModelFromFile(const fs::path& path) const noexcept;

private:
    const Buffer* vertexBuffer() const noexcept { return m_mngr->get(m_buffers.vertex); }
    const Buffer* indexBuffer() const noexcept { return m_mngr->get(m_buffers.index); }

    [[nodiscard]] std::span<Vertex> loadVertices(const aiMesh* mesh) const noexcept;
    [[nodiscard]] std::span<GLuint> loadIndices(const aiMesh* mesh) const noexcept;
    [[nodiscard]] Handle<Texture> loadTexture(const aiScene* scene, const aiMesh* mesh, aiTextureType textureType,
        const fs::path& modelPath) const noexcept;

    ResourceManager* m_mngr;
    SceneBuffers m_buffers;
    Handle<Texture> m_defaultTexture;
    mutable std::vector<Handle<Model2>> m_models;

    friend class IndirectRenderer;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
