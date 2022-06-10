#pragma once

#include "AttributeData.hpp"
#include "VertexBuffer.hpp"
#include "VertexFormat.hpp"

#include <cstring>
#include <functional>
#include <mutex>
#include <memory>
#include <ranges>
#include <span>

//------------------------------------------------------------------------

template<typename T>
requires std::is_base_of_v<T, AttributeData>
class AttributeDataPool final {
public:
    struct SpanBaseVertex {
        std::span<T> span;
        GLint basevertex;

        auto begin() noexcept { return span.begin(); }
        auto end() noexcept { return span.end(); }
    }

    AttributeDataPool()
          m_GPUPool{VertexBuffer(sizeof(T) * s_maxSize)}
    {}

    ~AttributeDataPool() = default;
    AttributeDataPool(const AttributeDataPool&) = delete;
    AttributeDataPool(AttributeDataPool&& other) = default;
    AttributeDataPool& operator=(const AttributeDataPool&) = delete;
    AttributeDataPool& operator=(AttributeDataPool&& other) = default;

    [[nodiscard]] SpanBaseVertex allocate(const std::size_t n, const T* data = nullptr)
    {
        std::lock_guard lg{m_mtx};
    
        if (m_offset + n > s_maxSize)
            throw std::bad_alloc();

        if (data != nullptr)
        {
            std::memcpy(&m_pool[m_offset], data, sizeof(T) * n);
            m_GPUPool.setData(data, sizeof(T) * n, sizeof(T) * m_offset);
        }
        const auto offset = m_offset
        m_offset += n;

        return SpanBaseVertex{{&m_pool[offset], n}, offset};
    }

    void transform(SpanBaseVertex span, std::function<T(const T&)> fn)
    {
        std::transform(span.begin(), span.end(), span.begin(), fn);
        m_GPUPool.setData(&m_pool[span.offset], sizeof(T) * std::distance(span.begin(), span.end()), span.offset);
    }

    static constexpr std::size_t s_maxSize = 10000000;

private:
    std::mutex m_mtx;
    std::unique_ptr<T[]> m_pool = std::make_unique<T[]>(s_maxSize);
    VertexBuffer m_GPUPool;
    GLint m_offset = 0;
};

//------------------------------------------------------------------------
