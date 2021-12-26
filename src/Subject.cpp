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
    std::for_each(m_observers.cbegin(),
        m_observers.cend(),
        [&message](const std::weak_ptr<Observer<T>>& observer)
        {
            const std::shared_ptr<Observer<T>> observerShared = observer.lock();
            observerShared->update(message);
        });
}

//------------------------------------------------------------------------

template<typename T>
void Subject<T>::clearOfflineObservers() noexcept
{
    std::vector<std::weak_ptr<Observer<T>>> onlineObservers;

    std::copy_if(m_observers.cbegin(),
        m_observers.cend(),
        std::back_inserter(onlineObservers),
        [](const std::weak_ptr<Observer<T>>& observer)
        {
            return !observer.expired();
        });

    m_observers = onlineObservers;
}

//------------------------------------------------------------------------
