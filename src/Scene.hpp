#pragma once

#include "Buffer.hpp"
#include "Handle.hpp"
#include "Model.hpp"
#include "ResourceManager.hpp"
#include "Texture.hpp"
#include "common.hpp"

#include <filesystem>
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
          m_vertexBuffer{mngr->createBuffer(GL_ARRAY_BUFFER, 1 << 17)},
          m_indexBuffer{mngr->createBuffer(GL_ELEMENT_ARRAY_BUFFER, 1 << 17)}
    {}

    void addModelFromFile(const fs::path& path) const noexcept;

private:
    const Buffer* vertexBuffer() const noexcept { return m_mngr->get(m_vertexBuffer); }
    const Buffer* indexBuffer() const noexcept { return m_mngr->get(m_indexBuffer); }

    ResourceManager* m_mngr;
    Handle<Buffer> m_vertexBuffer;
    Handle<Buffer> m_indexBuffer;
    mutable std::vector<Handle<Texture>> m_textures;
    mutable std::vector<Handle<Model2>> m_models;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
