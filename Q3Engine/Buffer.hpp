#pragma once

#include <cstdint>
#include <utility>
#include <vector>

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
    DataBufferSampler(DataBuffer<T>& buffer) : buffer_(buffer) {}

    void* getValue(uint32_t index) override { return &buffer_[index]; }

private:
    DataBuffer<T>& buffer_;
};

}
