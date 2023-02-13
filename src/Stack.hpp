#pragma once

#include <memory>
#include <stdexcept>
#include <vector>

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
    Stack(size_t reserveSize = 0) { m_underlying.reserve(reserveSize); }
    ~Stack() = default;

    Stack(const Stack&) = default;
    Stack& operator=(const Stack&) = default;
    Stack(Stack&&) = default;
    Stack& operator=(Stack&&) = default;

    [[nodiscard]] size_t getSize() const noexcept { return m_size; }

    T& top()
    {
        if (m_size == 0) [[unlikely]]
            throw std::out_of_range("Top of an empty Stack");

        return m_underlying.at(m_size - 1);
    }

    [[nodiscard]] const T& top() const
    {
        if (m_size == 0) [[unlikely]]
            throw std::out_of_range("Top of an empty Stack");

        return m_underlying.at(m_size - 1);
    }

    void pop()
    {
        if (m_size == 0) [[unlikely]]
            throw std::out_of_range("Pop on an empty Stack");

        --m_size;
    }

    void push(const T& item) noexcept
    requires std::copyable<T>
    {
        if (m_size == m_underlying.size()) [[unlikely]]
            m_underlying.push_back(item);
        else
            m_underlying[m_size] = item;

        ++m_size;
    }

    void push(T&& item) noexcept
    requires std::movable<T>
    {
        if (m_size == m_underlying.size()) [[unlikely]]
            m_underlying.push_back(item);
        else
            m_underlying[m_size] = item;
        
        ++m_size;
    }

    template<typename... Args>
    requires std::constructible_from<T, Args...>
    void emplace(Args&& ...args) noexcept
    {
        if (m_size == m_underlying.size()) [[unlikely]]
            m_underlying.emplace_back(std::forward<Args>(args)...);
        else
            std::construct_at(&m_underlying.at(m_size), std::forward<Args>(args)...);

        ++m_size;
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

private:
    std::vector<T> m_underlying;
    size_t m_size = 0;

    template <typename U>
    friend class ObjectPool;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
