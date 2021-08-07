#pragma once

//------------------------------------------------------------------------

template<typename T>
class Observer {
public:
    Observer() = default;
    virtual ~Observer() = default;

    virtual void update(const T& message) noexcept = 0;
};

//------------------------------------------------------------------------
