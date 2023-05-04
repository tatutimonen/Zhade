#pragma once

#include "Buffer.hpp"
#include "Handle.hpp"
#include "Model.hpp"

#include <string_view>
#include <vector>

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

class ResourceManager;

//------------------------------------------------------------------------

class Scene
{
public:
    Scene(ResourceManager* mngr)
        : m_mngr{mngr},
          m_vertexBuffer{mngr->createBuffer(GL_ARRAY_BUFFER, 1 << 17)},
          m_mappedVertexBuffer{vertexBuffer()->mapRangeWhole()},
          m_indexBuffer{mngr->createBuffer(GL_ELEMENT_ARRAY_BUFFER, 1 << 17)},
          m_mappedIndexBuffer{indexBuffer()->mapRangeWhole()}
    {}

    void addNamedModel(std::string_view filename) const noexcept;

private:
    const Buffer* vertexBuffer() const noexcept { return m_mngr->get(m_vertexBuffer); }
    const Buffer* indexBuffer() const noexcept { return m_mngr->get(m_indexBuffer); }

    ResourceManager* m_mngr;
    Handle<Buffer> m_vertexBuffer;
    Vertex* m_mappedVertexBuffer;
    Handle<Buffer> m_indexBuffer;
    GLuint* m_mappedIndexBuffer;
    std::vector<Handle<Model2>> m_models;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
