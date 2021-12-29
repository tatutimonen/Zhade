#pragma once

#include "Observer.hpp"

#include <memory>
#include <vector>

//------------------------------------------------------------------------

template<typename T>
class Subject {
public:
    Subject() = default;
    Subject(const std::vector<std::weak_ptr<Observer<T>>>& observers)
        : m_observers{observers}
    {}
    virtual ~Subject() = default;

    void attach(const std::weak_ptr<Observer<T>>& observer) noexcept
    {
        m_observers.push_back(observer);
    }

    void notify(const T& message) const noexcept
    {
        for (const auto& observer : m_observers)
            observer.lock()->update(message);
    }
    
    void clearOfflineObservers() noexcept
    {
        std::vector<std::weak_ptr<Observer<T>>> onlineObservers;

        for (const auto& observer : m_observers)
            if (!observer.expired())
                onlineObservers.push_back(observer);

        m_observers = onlineObservers;
    }

private:
    std::vector<std::weak_ptr<Observer<T>>> m_observers;
};

//------------------------------------------------------------------------
