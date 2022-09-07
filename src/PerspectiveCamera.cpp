#include "PerspectiveCamera.hpp"

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

PerspectiveCamera::PerspectiveCamera(const App& app, const Settings& settings)
    : Camera{app, std::move(settings)}
{
    updateProjectivity();
}

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
