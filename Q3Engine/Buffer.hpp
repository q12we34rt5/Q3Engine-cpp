#pragma once

#include <cstdint>
#include <utility>
#include <vector>

namespace q3 {

template<typename T>
class Buffer {
public:
    Buffer() : width_(0), height_(0), buffer_() {}
    Buffer(uint32_t width, uint32_t height) 
        : width_(width), height_(height), buffer_(width * height, T()) {}
    template<typename U>
    Buffer(uint32_t width, uint32_t height, U&& value) 
        : width_(width), height_(height), buffer_(width * height, std::forward<U>(value)) {}

    T* operator[](uint32_t y) { return buffer_.data() + y * width_; }
    const T* operator[](uint32_t y) const { return buffer_.data() + y * width_; }

    template<typename U>
    void setValue(uint32_t x, uint32_t y, U&& value) { buffer_.data()[x + width_ * y] = std::forward<U>(value); }
    T& getValue(uint32_t x, uint32_t y) { return buffer_.data()[x + width_ * y]; }
    const T& getValue(uint32_t x, uint32_t y) const { return buffer_.data()[x + width_ * y]; }

    uint32_t getWidth() const { return width_; }
    uint32_t getHeight() const { return height_; }
    T* getData() { return buffer_.data(); }
    const T* getData() const { return buffer_.data(); }

protected:
    uint32_t width_;
    uint32_t height_;
    std::vector<T> buffer_;
};

}
