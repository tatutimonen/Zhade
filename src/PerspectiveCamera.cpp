#include "PerspectiveCamera.hpp"

//------------------------------------------------------------------------

PerspectiveCamera::PerspectiveCamera(std::weak_ptr<const App> app, std::unique_ptr<Settings> settings)
    : Camera{app, std::move(settings)}
{
    updateProjectivity();
}

//------------------------------------------------------------------------
