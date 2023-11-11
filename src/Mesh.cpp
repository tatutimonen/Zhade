#include "Mesh.hpp"

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

Mesh::Mesh(MeshDescriptor desc)
    : m_vertices{desc.vertices},
      m_indices{desc.indices},
      m_baseVertex{desc.baseVertex},
      m_firstIndex{desc.firstIndex},
      m_diffuse{desc.diffuse},
      m_model{desc.model}
{}

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
