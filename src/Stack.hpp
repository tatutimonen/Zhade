#pragma once

#include <format>
#include <optional>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <vector>
#include <iostream>

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------
// Because std::stack is horrendously slow and std::pmr::deque with
// a std::pmr::monotonic_buffer_resource is not always applicable as a remedy.

template<typename T>
class Stack
{
public:
    Stack(size_t maxSize) : m_maxSize{maxSize} { m_underlying.resize(m_maxSize); }
    ~Stack() = default;

    Stack(const Stack&) = default;
    Stack& operator=(const Stack&) = default;
    Stack(Stack&&) = default;
    Stack& operator=(Stack&&) = default;

    [[nodiscard]] size_t getSize() const noexcept { return m_size; }
    [[nodiscard]] size_t getMaxSize() const noexcept { return m_maxSize; }

    [[nodiscard]] std::optional<std::reference_wrapper<const T>> top() const noexcept
    {
        if (m_size == 0) [[unlikely]]
            return std::nullopt;

        return m_underlying.at(m_size - 1);
    }

    void pop() noexcept
    {
        if (m_size == 0) [[unlikely]]
            return;

        --m_size;
    }

    void push(const T& item) noexcept
    requires std::copyable<T>
    {
        if (m_size == m_maxSize) [[unlikely]]
            resize();

        m_underlying[m_size++] = item;
    }

    void push(T&& item) noexcept
    requires std::movable<T>
    {
        if (m_size == m_maxSize) [[unlikely]]
            resize();

        m_underlying[m_size++] = std::move(item);
    }

    template<typename... Args>
    requires std::is_constructible_v<T, Args...>
    void emplace(Args&& ...args) noexcept
    {
        if (m_size == m_maxSize) [[unlikely]]
            resize();

        new (&m_underlying[m_size++]) T(std::forward<Args>(args)...);
    }

    [[nodiscard]] T& at(size_t pos)
    {
        return m_underlying.at(pos);
    }

    [[nodiscard]] const T& at(size_t pos) const
    {
        return m_underlying.at(pos);
    }

    [[nodiscard]] T& operator[](size_t pos)
    {
        return m_underlying.at(pos);
    }

    void resize()
    {
        m_maxSize *= 2;
        m_underlying.resize(m_maxSize);
    }

private:
    size_t m_maxSize;
    std::vector<T> m_underlying;
    size_t m_size = 0;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
