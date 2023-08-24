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

struct SceneBuffers
{
    Handle<Buffer> vertexBuffer;
    Handle<Buffer> indexBuffer;
};

//------------------------------------------------------------------------

class Scene
{
public:
    Scene(ResourceManager* mngr)
        : m_mngr{mngr},
          m_buffers{
              .vertexBuffer = m_mngr->createBuffer({.byteSize = GIB_BYTES/2, .usage = BufferUsage::VERTEX}),
              .indexBuffer = m_mngr->createBuffer({.byteSize = GIB_BYTES/2, .usage = BufferUsage::INDEX}),
          },
          m_textures{Texture::makeDefault(mngr)}
    {}

    [[nodiscard]] std::span<Handle<Model2>> getModels() const noexcept { return m_models; }

    void addModelFromFile(const fs::path& path) const noexcept;

private:
    const Buffer* vertexBuffer() const noexcept { return m_mngr->get(m_buffers.vertexBuffer); }
    const Buffer* indexBuffer() const noexcept { return m_mngr->get(m_buffers.indexBuffer); }

    [[nodiscard]] std::span<Vertex> loadVertices(const aiMesh* mesh) const noexcept;
    [[nodiscard]] std::span<GLuint> loadIndices(const aiMesh* mesh) const noexcept;

    ResourceManager* m_mngr;
    SceneBuffers m_buffers;
    mutable std::vector<Handle<Model2>> m_models;
    mutable std::vector<Handle<Texture>> m_textures;

    friend class IndirectRenderer;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
