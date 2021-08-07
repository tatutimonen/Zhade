#include "OrthographicCamera.hpp"

//------------------------------------------------------------------------

OrthographicCamera::OrthographicCamera(std::unique_ptr<Settings> settings)
    : Camera{std::move(settings)}
{
    updateProjectivity();
}

//------------------------------------------------------------------------
