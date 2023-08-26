#include "Mesh.hpp"

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

Mesh::Mesh(MeshDescriptor desc)
    : m_vertices{desc.vertices},
      m_indices{desc.indices},
      m_diffuse{desc.diffuse}
{}

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
