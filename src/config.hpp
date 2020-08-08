#pragma once

#include <string>

namespace Config
{
    std::string const APP_NAME("Shadow Mapper"); // TODO: Setup setting this through CMake

    int constexpr OPENGL_VERSION_MAJOR = 3;
    int constexpr OPENGL_VERSION_MINOR = 3;
}