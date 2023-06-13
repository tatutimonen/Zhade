#pragma once

#include "Buffer.hpp"
#include "Handle.hpp"
#include "Model.hpp"
#include "ResourceManager.hpp"
#include "Texture.hpp"
#include "common.hpp"

#include <robin_hood.h>

#include <atomic>
#include <filesystem>
#include <string>
#include <vector>

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

class Scene
{
public:
    Scene(ResourceManager* mngr)
        : m_mngr{mngr},
          m_vertexBuffer{mngr->createBuffer({.sizeBytes = 1 << 28, .usage = BufferUsage::VERTEX})},
          m_indexBuffer{mngr->createBuffer({.sizeBytes = 1 << 28, .usage = BufferUsage::INDEX})},
          m_textures{Texture::makeDefault(mngr)}
    {}

    [[nodiscard]] std::span<Handle<Model2>> getModels() const noexcept { return m_models; }

    void addModelFromFile(const fs::path& path) const noexcept;

private:
    const Buffer* vertexBuffer() const noexcept { return m_mngr->get(m_vertexBuffer); }
    const Buffer* indexBuffer() const noexcept { return m_mngr->get(m_indexBuffer); }

    ResourceManager* m_mngr;
    Handle<Buffer> m_vertexBuffer;
    Handle<Buffer> m_indexBuffer;
    mutable std::vector<Handle<Texture>> m_textures;
    mutable uint32_t m_diffuseID{1};
    mutable std::vector<Handle<Model2>> m_models;
    mutable robin_hood::unordered_map<std::string, Handle<Model2>> m_modelCache;
    mutable std::atomic_uint32_t m_modelID;

    friend class IndirectRenderer;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
