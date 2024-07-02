#pragma once

#include "RGBColor.hpp"
#include "Math.hpp"

#include <cstdint>

namespace q3 {

struct Triangle {
    const Vector3& v0;
    const Vector3& v1;
    const Vector3& v2;
};

struct Barycentric {
    float l0, l1, l2;
};

class Shader {
public:
    virtual std::size_t getContextSize() const = 0;
    virtual bool vertexShader(Vertex& v0, Vertex& v1, Vertex& v2, void* data0, void* data1, void* data2, void* context) = 0;
    virtual RGBColor fragmentShader(const Triangle& triangle, const Barycentric& barycentric, void* data0, void* data1, void* data2, const void* context) = 0;
};

}
