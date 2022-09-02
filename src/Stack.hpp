#pragma once

#include <concepts>
#include <functional>
#include <memory>
#include <optional>
#include <type_traits>

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------
// Because std::stack is horrendously slow and std::pmr::deque with
// a std::pmr::monotonic_buffer_resource is not always applicable as a remedy.

template<typename T>
requires std::copyable<T> || std::movable<T>
class Stack
{
public:
    Stack(size_t maxSize)
        : m_maxSize{maxSize},
          m_underlying{std::make_unique<T[]>(m_maxSize)}
    {}

    ~Stack() = default;

    Stack(const Stack&) = default;
    Stack& operator=(const Stack&) = default;
    Stack(Stack&&) = default;
    Stack& operator=(Stack&&) = default;

    [[nodiscard]] size_t getSize() const noexcept { return m_size; }
    [[nodiscard]] size_t getMaxSize() const noexcept { return m_maxSize; }

    [[nodiscard]] std::optional<std::reference_wrapper<const T>> top() const noexcept
    {
        if (m_size == 0)
            return std::nullopt;

        return m_underlying[m_size - 1];
    }

    bool pop() noexcept
    {
        if (m_size == 0)
            return false;

        --m_size;
        return true;
    }

    bool push(const T& item) noexcept
    requires std::copyable<T>
    {
        if (m_size == m_maxSize)
            return false;

        m_underlying[m_size++] = item;
        return true;
    }

    bool push(T&& item) noexcept
    requires std::movable<T>
    {
        if (m_size == m_maxSize)
            return false;

        m_underlying[m_size++] = std::move(item);
        return true;
    }

    template<typename... Args>
    bool pushEmplace(Args&& ...args) noexcept
    requires std::is_constructible_v<T, Args...>
    {
        if (m_size == m_maxSize)
            return false;

        m_underlying[m_size++] = T(std::forward<Args>(args)...);
        return true;
    }

private:
    const size_t m_maxSize;
    std::unique_ptr<T[]> m_underlying;
    size_t m_size = 0;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
