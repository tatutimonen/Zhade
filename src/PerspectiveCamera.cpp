#include "PerspectiveCamera.hpp"

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

PerspectiveCamera::PerspectiveCamera(const App& app, std::unique_ptr<Settings> settings)
    : Camera{app, std::move(settings)}
{
    updateProjectivity();
}

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
