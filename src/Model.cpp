#include "Model.hpp"

#include "ResourceManager.hpp"

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

Model::Model(ModelDescriptor desc)
    : m_meshes{desc.meshes},
      m_textures{desc.textures},
      m_transformation{desc.transformation},
      m_mngr{desc.mngr},
      m_managed{desc.managed}
{}

//------------------------------------------------------------------------

Model::~Model()
{
    if (m_managed) [[likely]] return;
    freeResources();
}

//------------------------------------------------------------------------

void Model::freeResources() const noexcept
{
    for (const auto& texHandle : m_textures) {
        m_mngr->destroy(texHandle);
    }
    for (auto& mesh : m_meshes) {
        mesh.alive = false;
    }
}

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
