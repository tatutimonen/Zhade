#pragma once

#include <glm/glm.hpp>

//------------------------------------------------------------------------

template<typename T>
class Observer {
public:
    Observer() = default;
    virtual ~Observer() = default;

    virtual void update(const T& message) noexcept = 0;
};

namespace observed
{

using CameraCenter = glm::vec3;

}  // namespace observed

//------------------------------------------------------------------------
