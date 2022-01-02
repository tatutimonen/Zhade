#pragma once

#include "Texture.hpp"

#include <stb_image.h>

#include <cstdint>
#include <string>

//------------------------------------------------------------------------

class Texture2D final : public Texture {
public:
    struct Settings : Texture::Settings {
        int32_t height = 1;

        uint32_t wrapT = GL_CLAMP_TO_EDGE;

        Settings() = default;
        Settings(int32_t width, int32_t height)
            : Texture::Settings(width),
              height{height}
        {}
    };

    Texture2D(const std::string& filename);
    Texture2D(const std::shared_ptr<Settings>& settings);
    ~Texture2D();

    virtual void setData(const void* data) noexcept override;
    virtual void setParameteri(uint32_t pname, int32_t param) const noexcept override;
    virtual inline const Settings& getSettings() const noexcept override { return *m_settings; }
    virtual inline uint32_t getHandle() const noexcept override          { return m_handle; }

    virtual inline void bind() const noexcept override   { glBindTexture(GL_TEXTURE_2D, m_handle); }
    virtual inline void unbind() const noexcept override { glBindTexture(GL_TEXTURE_2D, 0); }

    static std::unique_ptr<Texture2D> makeDefault() noexcept;

private:
    void setupTexture() noexcept;

    uint32_t m_handle;
    std::shared_ptr<Settings> m_settings;
};

//------------------------------------------------------------------------
