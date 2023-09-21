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
      m_transformation{desc.transformation},
      m_id{desc.id}
{}

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
