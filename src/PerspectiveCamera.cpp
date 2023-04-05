#include "PerspectiveCamera.hpp"

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

PerspectiveCamera::PerspectiveCamera(ResourceManager* mngr, App* app, const Settings& settings)
    : Camera{mngr, app, std::move(settings)}
{
    updateProjectivity();
}

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
