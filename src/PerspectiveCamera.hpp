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

        Settings(std::unique_ptr<Camera::Settings> baseSettings = std::make_unique<Camera::Settings>(), float fov = 70.0f,
                 float aspectRatio = static_cast<float>(App::s_windowWidth) / App::s_windowHeight)
            : Camera::Settings(std::move(*baseSettings)),
              fov{fov},
              aspectRatio{aspectRatio}
        {
            baseSettings.release();
        }
    };

    PerspectiveCamera(const App& app, std::unique_ptr<Settings> settings = std::make_unique<Settings>());

    virtual void updateProjectivity() override
    {
        const auto& settings = dynamic_cast<const Settings&>(getSettings());
        m_matrices.P = glm::perspective(settings.fov, settings.aspectRatio, settings.zNear, settings.zFar);
    }

    virtual inline const Camera::Settings& getSettings() const noexcept override { return *m_settings; }
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
