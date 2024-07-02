#pragma once

#include "RGBColor.hpp"
#include "Math.hpp"

#include <cstdint>

namespace q3 {

class Shader {
public:
    virtual std::size_t getContextSize() const = 0;
    virtual bool vertexShader(Vertex& v0, Vertex& v1, Vertex& v2, void* data0, void* data1, void* data2, void* context) = 0;
    virtual RGBColor fragmentShader(const Triangle& triangle, const Barycentric& barycentric, void* data0, void* data1, void* data2, const void* context) = 0;
};

}
