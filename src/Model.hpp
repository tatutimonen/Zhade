#pragma once

#include "Handle.hpp"
#include "Texture.hpp"
#include "common.hpp"

#include <span>
#include <vector>

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------
    
class ResourceManager;

struct ModelDescriptor
{
    std::span<Mesh> meshes;
    std::vector<Handle<Texture>> textures;
    glm::mat4 mat{1.0f};
    ResourceManager* mngr = nullptr;
};

//------------------------------------------------------------------------

class Model
{
public:
    Model() = default;
    explicit Model(ModelDescriptor desc);
    ~Model();

    Model(const Model&) = default;
    Model& operator=(const Model&) = default;
    Model(Model&&) = delete;
    Model& operator=(Model&&) = delete;

    void freeResources();

private:
    std::span<Mesh> m_meshes;
    std::vector<Handle<Texture>> m_textures;
    glm::mat4 m_mat;
    ResourceManager* m_mngr = nullptr;

    friend class Scene;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
