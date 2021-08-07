#include "PerspectiveCamera.hpp"

//------------------------------------------------------------------------

PerspectiveCamera::PerspectiveCamera(std::unique_ptr<Settings> settings)
    : Camera{std::move(settings)}
{
    updateProjectivity();
}

//------------------------------------------------------------------------
