#include "Camera.hpp"

//------------------------------------------------------------------------

class PerspectiveCamera final : public Camera {
public:
    struct Settings final : Camera::Settings {
        float fov;
        float aspectRatio;

        Settings(std::unique_ptr<Camera::Settings> baseSettings = std::make_unique<Camera::Settings>(),
            float fov = 70.0f,
            float aspectRatio = static_cast<float>(App::window_width) / static_cast<float>(App::window_height))
            : Camera::Settings(std::move(*baseSettings)),
              fov{fov},
              aspectRatio{aspectRatio}
        {
            baseSettings.release();
        }
    };

    PerspectiveCamera(std::unique_ptr<Settings> settings = std::make_unique<Settings>());

    virtual inline void updateProjectivity() override
    {
        const Settings& settings = dynamic_cast<const Settings&>(getSettings());
        m_projectivity = glm::perspective(settings.fov, settings.aspectRatio, settings.zNear, settings.zFar);
    }

    virtual inline const Camera::Settings& getSettings() const { return *m_settings; }
};

//------------------------------------------------------------------------
