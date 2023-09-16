#include "Model.hpp"

#include "ResourceManager.hpp"

#include <bit>
#include <numeric>

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

Model2::Model2(ModelDescriptor desc)
    : m_mngr{desc.mngr},
      m_meshes{desc.meshes},
      m_transformation{desc.transformation}
{}

//------------------------------------------------------------------------

[[nodiscard]] const Mesh* Model2::getFirstMesh() const noexcept
{
    const Mesh* ptr = m_meshes.size() > 0 ? m_mngr->get(m_meshes.front()) : nullptr;
    return ptr != nullptr ? ptr : std::bit_cast<const Mesh*>(std::numeric_limits<uint64_t>::max());
}

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
