#include "PerspectiveCamera.hpp"

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

PerspectiveCamera::PerspectiveCamera(ResourceManager& mngr, const App& app, const Settings& settings)
    : Camera{mngr, app, std::move(settings)}
{
    updateProjectivity();
}

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
