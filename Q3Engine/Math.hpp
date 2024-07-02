#pragma once

namespace q3 {

template<typename T>
class Vector3T {
public:
    constexpr Vector3T() : x(0), y(0), z(0) {}
    constexpr Vector3T(T x, T y, T z) : x(x), y(y), z(z) {}

    // +, -, *, /
    constexpr Vector3T<T> operator+(const Vector3T<T>& other) const noexcept { return {x + other.x, y + other.y, z + other.z}; }
    constexpr Vector3T<T> operator-(const Vector3T<T>& other) const noexcept { return {x - other.x, y - other.y, z - other.z}; }
    constexpr Vector3T<T> operator*(const Vector3T<T>& other) const noexcept { return {x * other.x, y * other.y, z * other.z}; }
    constexpr Vector3T<T> operator/(const Vector3T<T>& other) const noexcept { return {x / other.x, y / other.y, z / other.z}; }
    constexpr Vector3T<T> operator+(T scalar) const noexcept { return {x + scalar, y + scalar, z + scalar}; }
    constexpr Vector3T<T> operator-(T scalar) const noexcept { return {x - scalar, y - scalar, z - scalar}; }
    constexpr Vector3T<T> operator*(T scalar) const noexcept { return {x * scalar, y * scalar, z * scalar}; }
    constexpr Vector3T<T> operator/(T scalar) const noexcept { return {x / scalar, y / scalar, z / scalar}; }
    // +=, -=, *=, /=
    constexpr Vector3T<T>& operator+=(const Vector3T<T>& other) noexcept { x += other.x; y += other.y; z += other.z; return *this; }
    constexpr Vector3T<T>& operator-=(const Vector3T<T>& other) noexcept { x -= other.x; y -= other.y; z -= other.z; return *this; }
    constexpr Vector3T<T>& operator*=(const Vector3T<T>& other) noexcept { x *= other.x; y *= other.y; z *= other.z; return *this; }
    constexpr Vector3T<T>& operator/=(const Vector3T<T>& other) noexcept { x /= other.x; y /= other.y; z /= other.z; return *this; }
    constexpr Vector3T<T>& operator+=(T scalar) noexcept { x += scalar; y += scalar; z += scalar; return *this; }
    constexpr Vector3T<T>& operator-=(T scalar) noexcept { x -= scalar; y -= scalar; z -= scalar; return *this; }
    constexpr Vector3T<T>& operator*=(T scalar) noexcept { x *= scalar; y *= scalar; z *= scalar; return *this; }
    constexpr Vector3T<T>& operator/=(T scalar) noexcept { x /= scalar; y /= scalar; z /= scalar; return *this; }
    // negation
    constexpr Vector3T<T> operator-() const noexcept { return {-x, -y, -z}; }
    // comparison
    constexpr bool operator==(const Vector3T<T>& other) const noexcept { return x == other.x && y == other.y && z == other.z; }
    // dot product
    constexpr T dot(const Vector3T<T>& other) const noexcept { return x * other.x + y * other.y + z * other.z; }

    T x, y, z;
};

template<typename T>
class Vector2T {
public:
    constexpr Vector2T() : x(0), y(0) {}
    constexpr Vector2T(T x, T y) : x(x), y(y) {}
    // from Vector3
    constexpr explicit Vector2T(const Vector3T<T>& other) : x(other.x), y(other.y) {}

    // +, -, *, /
    constexpr Vector2T<T> operator+(const Vector2T<T>& other) const noexcept { return {x + other.x, y + other.y}; }
    constexpr Vector2T<T> operator-(const Vector2T<T>& other) const noexcept { return {x - other.x, y - other.y}; }
    constexpr Vector2T<T> operator*(const Vector2T<T>& other) const noexcept { return {x * other.x, y * other.y}; }
    constexpr Vector2T<T> operator/(const Vector2T<T>& other) const noexcept { return {x / other.x, y / other.y}; }
    constexpr Vector2T<T> operator+(T scalar) const noexcept { return {x + scalar, y + scalar}; }
    constexpr Vector2T<T> operator-(T scalar) const noexcept { return {x - scalar, y - scalar}; }
    constexpr Vector2T<T> operator*(T scalar) const noexcept { return {x * scalar, y * scalar}; }
    constexpr Vector2T<T> operator/(T scalar) const noexcept { return {x / scalar, y / scalar}; }
    // +=, -=, *=, /=
    constexpr Vector2T<T>& operator+=(const Vector2T<T>& other) noexcept { x += other.x; y += other.y; return *this; }
    constexpr Vector2T<T>& operator-=(const Vector2T<T>& other) noexcept { x -= other.x; y -= other.y; return *this; }
    constexpr Vector2T<T>& operator*=(const Vector2T<T>& other) noexcept { x *= other.x; y *= other.y; return *this; }
    constexpr Vector2T<T>& operator/=(const Vector2T<T>& other) noexcept { x /= other.x; y /= other.y; return *this; }
    constexpr Vector2T<T>& operator+=(T scalar) noexcept { x += scalar; y += scalar; return *this; }
    constexpr Vector2T<T>& operator-=(T scalar) noexcept { x -= scalar; y -= scalar; return *this; }
    constexpr Vector2T<T>& operator*=(T scalar) noexcept { x *= scalar; y *= scalar; return *this; }
    constexpr Vector2T<T>& operator/=(T scalar) noexcept { x /= scalar; y /= scalar; return *this; }
    // negation
    constexpr Vector2T<T> operator-() const noexcept { return {-x, -y}; }
    // comparison
    constexpr bool operator==(const Vector2T<T>& other) const noexcept { return x == other.x && y == other.y; }
    // dot product
    constexpr T dot(const Vector2T<T>& other) const noexcept { return x * other.x + y * other.y; }

    T x, y;
};

using Vector3 = Vector3T<float>;
using Vector2 = Vector2T<float>;

using Vector3i = Vector3T<int32_t>;
using Vector2i = Vector2T<int32_t>;

class Vertex {
public:
    constexpr Vertex() : position(), w(1) {}
    constexpr Vertex(const Vector3& position) : position(position), w(1) {}
    constexpr Vertex(const Vector3& position, float w) : position(position), w(w) {}
    constexpr Vertex(float x, float y, float z) : position(x, y, z), w(1) {}
    constexpr Vertex(float x, float y, float z, float w) : position(x, y, z), w(w) {}

    Vector3 position;
    float w;
};

struct Triangle {
    const Vector3& v0;
    const Vector3& v1;
    const Vector3& v2;
};

struct Barycentric {
    float l0, l1, l2;
};

inline constexpr Barycentric calculateBarycentric(const Triangle& triangle, const Vector2& p) {
    q3::Vector2 v0 = q3::Vector2(triangle.v1 - triangle.v0);
    q3::Vector2 v1 = q3::Vector2(triangle.v2 - triangle.v0);
    q3::Vector2 v2 = p - q3::Vector2(triangle.v0);
    float d00 = v0.dot(v0);
    float d01 = v0.dot(v1);
    float d11 = v1.dot(v1);
    float d20 = v2.dot(v0);
    float d21 = v2.dot(v1);
    float denom = d00 * d11 - d01 * d01;
    float l1 = (d11 * d20 - d01 * d21) / denom;
    float l2 = (d00 * d21 - d01 * d20) / denom;
    float l0 = 1.0f - l1 - l2;
    return {l0, l1, l2};
}

}
