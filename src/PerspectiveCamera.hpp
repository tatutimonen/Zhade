#include "Camera.hpp"

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

class PerspectiveCamera final : public Camera
{
public:
    struct Settings final : Camera::Settings
    {
        float fov;
        float aspectRatio;

        Settings(const Camera::Settings& baseSettings = Camera::Settings(), float fov = 70.0f,
            float aspectRatio = static_cast<float>(App::s_windowWidth) / App::s_windowHeight)
            : Camera::Settings(std::move(baseSettings)),
              fov{fov},
              aspectRatio{aspectRatio}
        {}
    };

    PerspectiveCamera(ResourceManager* mngr, const App* app, const Settings& settings = Settings());

    virtual void updateProjectivity() const noexcept override
    {
        const auto& settings = static_cast<Settings>(getSettings());
        m_matrices.P = glm::perspective(settings.fov, settings.aspectRatio, settings.zNear, settings.zFar);
    }

    virtual const Camera::Settings& getSettings() const noexcept override { return m_settings; }
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
