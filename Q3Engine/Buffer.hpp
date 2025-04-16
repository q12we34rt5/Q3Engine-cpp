#pragma once

#include <cstdint>
#include <utility>
#include <vector>
#include <stdexcept>
#include <type_traits>
#include <tuple>
#include <memory>

namespace q3 {

template<typename T>
class GraphicsBuffer {
public:
    GraphicsBuffer() : width_(0), height_(0), data_() {}
    GraphicsBuffer(uint32_t width, uint32_t height) 
        : width_(width), height_(height), data_(width * height, T()) {}
    template<typename U>
    GraphicsBuffer(uint32_t width, uint32_t height, U&& value) 
        : width_(width), height_(height), data_(width * height, std::forward<U>(value)) {}
    GraphicsBuffer(const std::vector<T>& data, uint32_t width, uint32_t height)
        : width_(width), height_(height), data_(data) {
        if (data_.size() != width * height) { throw std::invalid_argument("Data size does not match the specified width and height."); }
    }
    GraphicsBuffer(std::vector<T>&& data, uint32_t width, uint32_t height)
        : width_(width), height_(height), data_(std::move(data)) {
        if (data_.size() != width * height) { throw std::invalid_argument("Data size does not match the specified width and height."); }
    }

    T* operator[](uint32_t y) { return data_.data() + y * width_; }
    const T* operator[](uint32_t y) const { return data_.data() + y * width_; }

    template<typename U>
    void setValue(uint32_t x, uint32_t y, U&& value) { data_.data()[x + width_ * y] = std::forward<U>(value); }
    T& getValue(uint32_t x, uint32_t y) { return data_.data()[x + width_ * y]; }
    const T& getValue(uint32_t x, uint32_t y) const { return data_.data()[x + width_ * y]; }

    template<typename U>
    void fill(U&& value) { std::fill(data_.begin(), data_.end(), std::forward<U>(value)); }

    uint32_t getWidth() const { return width_; }
    uint32_t getHeight() const { return height_; }
    T* getData() { return data_.data(); }
    const T* getData() const { return data_.data(); }

protected:
    uint32_t width_;
    uint32_t height_;
    std::vector<T> data_;
};

template<typename T>
using DataBuffer = std::vector<T>;

class BaseDataBufferSampler {
public:
    virtual void* getValue(uint32_t index) = 0;
};

class DummyDataBufferSampler : public BaseDataBufferSampler {
public:
    void* getValue(uint32_t index) override { return nullptr; }
};

template<typename T>
class DataBufferSampler : public BaseDataBufferSampler {
public:
    DataBufferSampler(std::shared_ptr<DataBuffer<T>> buffer) : buffer_(buffer) {}

    void* getValue(uint32_t index) override { return &buffer_->operator[](index); }

private:
    std::shared_ptr<DataBuffer<T>> buffer_;
};

/**
 * @brief A flexible sampler for multiple data buffers of different types.
 *
 * AutoDataBufferSampler provides a type-safe and efficient way to access
 * multiple std::shared_ptr<q3::DataBuffer<T>> instances simultaneously. It aggregates
 * element-wise references from all buffers into a std::tuple, enabling safe and
 * structured access to per-element data across multiple streams.
 *
 * Key features:
 * - Supports any number of std::shared_ptr<q3::DataBuffer<T>> buffers
 * - Aggregates data as std::tuple<T1&, T2&, ...> for zero-overhead reference access
 * - Ensures all buffers are non-null and of equal size at runtime
 * - Compatible with the q3::BaseDataBufferSampler interface
 *
 * Usage example:
 * @code
 * auto uvs     = std::make_shared<q3::DataBuffer<q3::Vector2>>({{0, 1}});
 * auto normals = std::make_shared<q3::DataBuffer<q3::Vector3>>({{0, 1, 0}});
 *
 * AutoDataBufferSampler sampler(uvs, normals);
 *
 * // Access combined data as a tuple of references
 * auto& [uv, normal] = *static_cast<std::tuple<q3::Vector2&, q3::Vector3&>*>(sampler.getValue(0));
 *
 * // Now you can directly use uv and normal
 * @endcode
 *
 * @note When using getValue(), ensure:
 * 1. You cast the returned pointer to the correct std::tuple type.
 * 2. The number, order, and types in the tuple exactly match the buffers passed in.
 */
class AutoDataBufferSampler : public BaseDataBufferSampler {
    // abstract base class providing a generic interface for buffer access
    class BaseBufferWrapper {
    public:
        virtual void* getValue(uint32_t index) = 0;
    };

    // template class for storing and accessing multiple buffers
    template<typename... Buffers>
    class BufferWrapper : public BaseBufferWrapper {
        using Data = std::tuple<typename Buffers::reference...>;

        // helper function to initialize the data vector
        template<std::size_t... I>
        void initHelper(std::index_sequence<I...>) {
            for (std::size_t i = 0; i < std::get<0>(buffers_)->size(); i++) {
                data.emplace_back(Data(std::get<I>(buffers_)->operator[](i)...));
            }
        }

    public:
        // takes references to multiple buffers
        BufferWrapper(std::shared_ptr<Buffers>... buffers) : buffers_{buffers...}, data{} {
            data.reserve(std::get<0>(buffers_)->size());
            initHelper(std::make_index_sequence<sizeof...(Buffers)>());
        }

        void* getValue(uint32_t index) override {
            return &data[index];
        }

    private:
        std::tuple<std::shared_ptr<Buffers>...> buffers_;
        std::vector<Data> data; // storing the combined data
    };

    // helper function to get the size of the first buffer
    template<typename FirstBuffer, typename... RestBuffers>
    static std::size_t getFirstBufferSize(std::shared_ptr<FirstBuffer>& first, std::shared_ptr<RestBuffers>&...) {
        return first->size();
    }

public:
    AutoDataBufferSampler() : buffers_(nullptr) {}

    // accepts any number of DataBuffers
    template<typename... Buffers>
    AutoDataBufferSampler(std::shared_ptr<Buffers>... buffers) {
        setBuffers(buffers...);
    }

    template<typename... Buffers>
    void setBuffers(std::shared_ptr<Buffers>... buffers) {
        // ensure at least one buffer is provided
        static_assert(sizeof...(Buffers) > 0, "At least one buffer is required");
        // ensure all provided buffers are of type std::shared_ptr<q3::DataBuffer<T>>
        static_assert((std::is_same_v<DataBuffer<typename std::remove_reference_t<Buffers>::value_type>, Buffers> && ...), "All buffers must be of type std::shared_ptr<q3::DataBuffer<T>>");
        // ensure all provided buffers are not nullptr
        if (((buffers == nullptr) || ...)) { throw std::invalid_argument("All buffers must be non-null"); }
        // ensure all buffers have the same size
        auto buffer_size = getFirstBufferSize(buffers...);
        if (!((buffers->size() == buffer_size) && ...)) { throw std::invalid_argument("All buffers must have the same size"); }

        buffers_ = std::make_unique<BufferWrapper<Buffers...>>(buffers...);
    }

    void* getValue(uint32_t index) override {
        return buffers_->getValue(index);
    }

private:
    std::unique_ptr<BaseBufferWrapper> buffers_; // polymorphic storage for BufferWrapper
};

}
