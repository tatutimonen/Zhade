#pragma once

#include "Buffer.hpp"
#include "Handle.hpp"
#include "Model.hpp"
#include "ResourceManager.hpp"
#include "Texture.hpp"
#include "common.hpp"

#include <assimp/scene.h>

#include <bitset>
#include <filesystem>
#include <string>
#include <vector>

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

constexpr std::bitset<AI_TEXTURE_TYPE_MAX> makeSupportedTextureTypeTable()
{
    std::bitset<AI_TEXTURE_TYPE_MAX> table{};
    table.set(aiTextureType_DIFFUSE);
    return table;
}

inline constexpr auto SUPPORTED_TEXTURE_TYPES = makeSupportedTextureTypeTable();

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
    [[nodiscard]] const Buffer* vertexBuffer() const noexcept { return m_mngr->get(m_vertexBuffer); }
    [[nodiscard]] const Buffer* indexBuffer() const noexcept { return m_mngr->get(m_indexBuffer); }

    [[nodiscard]] MeshDescriptor loadMeshConstituentsAsync(const aiScene* scene, const aiMesh* mesh,
        const std::bitset<AI_TEXTURE_TYPE_MAX>& textureTypes, const fs::path& basePath) const noexcept;
    [[nodiscard]] std::span<Vertex> loadVertices(const aiMesh* mesh) const noexcept;
    [[nodiscard]] std::span<GLuint> loadIndices(const aiMesh* mesh) const noexcept;
    [[nodiscard]] Handle<Texture> loadTexture(const aiScene* scene, const aiMesh* mesh,
        aiTextureType textureType, const fs::path& path) const noexcept;

    ResourceManager* m_mngr;
    Handle<Buffer> m_vertexBuffer;
    Handle<Buffer> m_indexBuffer;
    Handle<Texture> m_defaultTexture;
    mutable std::vector<Handle<Model2>> m_models;

    friend class IndirectRenderer;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
