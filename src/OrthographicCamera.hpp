#include "Camera.hpp"

//------------------------------------------------------------------------

class OrthographicCamera final : public Camera {
public:
    struct Settings final : Camera::Settings {
        glm::vec2 xRange;
        glm::vec2 yRange;

        Settings(std::unique_ptr<Camera::Settings> baseSettings = std::make_unique<Camera::Settings>(),
            const glm::vec2& xRange = glm::vec2(-5.0f, 5.0f),
            const glm::vec2& yRange = glm::vec2(-5.0f, 5.0f))
            : Camera::Settings(std::move(*baseSettings)),
              xRange{xRange},
              yRange{yRange}
        {
            baseSettings.release();
        }
    };

    OrthographicCamera(std::unique_ptr<Settings> settings = std::make_unique<Settings>());

    virtual inline void updateProjectivity() override
    {
        const Settings& settings = dynamic_cast<const Settings&>(getSettings());
        m_matrices.P = glm::ortho(
            settings.xRange[0],
            settings.xRange[1],
            settings.yRange[0],
            settings.yRange[1],
            settings.zNear,
            settings.zFar);
    }

    virtual inline const Camera::Settings& getSettings() const { return *m_settings; }
};

//------------------------------------------------------------------------
