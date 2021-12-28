#include "Subject.hpp"

//------------------------------------------------------------------------

template<typename T>
Subject<T>::Subject(const std::vector<std::weak_ptr<Observer<T>>>& observers)
    : m_observers{observers}
{}

//------------------------------------------------------------------------

template<typename T>
void Subject<T>::attach(const std::weak_ptr<Observer<T>>& observer) noexcept
{
    m_observers.push_back(observer);
}

//------------------------------------------------------------------------

template<typename T>
void Subject<T>::notify(const T& message) const noexcept
{
    for (const auto& observer : m_observers)
        observer.lock()->update(message);
}

//------------------------------------------------------------------------

template<typename T>
void Subject<T>::clearOfflineObservers() noexcept
{
    std::vector<std::weak_ptr<Observer<T>>> onlineObservers;

    for (const auto& observer : m_observers)
        if (!observer.expired())
            onlineObservers.push_back(observer);

    m_observers = onlineObservers;
}

//------------------------------------------------------------------------
