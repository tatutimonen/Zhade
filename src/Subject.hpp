#pragma once

#include "Observer.hpp"

#include <memory>
#include <vector>

//------------------------------------------------------------------------

template<typename T>
class Subject {
public:
    Subject() = default;
    Subject(const std::vector<std::weak_ptr<Observer<T>>>& observers);
    virtual ~Subject() = default;

    void attach(const std::weak_ptr<Observer<T>>& observer) noexcept;
    void notify(const T& message) const noexcept;
    void clearOfflineObservers() noexcept;

private:
    std::vector<std::weak_ptr<Observer<T>>> m_observers;
};

//------------------------------------------------------------------------
