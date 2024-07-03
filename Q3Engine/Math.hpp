#pragma once

namespace q3 {

template<typename T> class Vector3T;
template<typename T> class Vector4T;

template<typename T>
class Vector2T {
public:
    constexpr Vector2T() noexcept : x(), y() {}
    constexpr Vector2T(T x, T y) noexcept : x(x), y(y) {}
    // from Vector3
    constexpr explicit Vector2T(const Vector3T<T>& other) noexcept;

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

template<typename T>
constexpr Vector2T<T>::Vector2T(const Vector3T<T>& other) noexcept : x(other.x), y(other.y) {}

template<typename T>
class Vector3T {
public:
    constexpr Vector3T() noexcept : x(), y(), z() {}
    constexpr Vector3T(T x, T y, T z) noexcept : x(x), y(y), z(z) {}
    constexpr Vector3T(const Vector2T<T>& other, T z) noexcept : x(other.x), y(other.y), z(z) {}
    constexpr Vector3T(const Vector2T<T>& other) noexcept : x(other.x), y(other.y), z() {}
    // from Vector4
    constexpr explicit Vector3T(const Vector4T<T>& other) noexcept;

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
constexpr Vector3T<T>::Vector3T(const Vector4T<T>& other) noexcept : x(other.position.x), y(other.position.y), z(other.position.z) {}

template<typename T>
class Vector4T {
public:
    constexpr Vector4T() noexcept : position(), w() {}
    constexpr Vector4T(T x, T y, T z, T w) noexcept : position(x, y, z), w(w) {}
    constexpr Vector4T(const Vector3T<T>& other, T w) noexcept : position(other), w(w) {}
    constexpr Vector4T(const Vector3T<T>& other) noexcept : position(other), w() {}

    // +, -, *, /
    constexpr Vector4T<T> operator+(const Vector4T<T>& other) const noexcept { return {position + other.position, w + other.w}; }
    constexpr Vector4T<T> operator-(const Vector4T<T>& other) const noexcept { return {position - other.position, w - other.w}; }
    constexpr Vector4T<T> operator*(const Vector4T<T>& other) const noexcept { return {position * other.position, w * other.w}; }
    constexpr Vector4T<T> operator/(const Vector4T<T>& other) const noexcept { return {position / other.position, w / other.w}; }
    constexpr Vector4T<T> operator+(T scalar) const noexcept { return {position + scalar, w + scalar}; }
    constexpr Vector4T<T> operator-(T scalar) const noexcept { return {position - scalar, w - scalar}; }
    constexpr Vector4T<T> operator*(T scalar) const noexcept { return {position * scalar, w * scalar}; }
    constexpr Vector4T<T> operator/(T scalar) const noexcept { return {position / scalar, w / scalar}; }
    // +=, -=, *=, /=
    constexpr Vector4T<T>& operator+=(const Vector4T<T>& other) noexcept { position += other.position; w += other.w; return *this; }
    constexpr Vector4T<T>& operator-=(const Vector4T<T>& other) noexcept { position -= other.position; w -= other.w; return *this; }
    constexpr Vector4T<T>& operator*=(const Vector4T<T>& other) noexcept { position *= other.position; w *= other.w; return *this; }
    constexpr Vector4T<T>& operator/=(const Vector4T<T>& other) noexcept { position /= other.position; w /= other.w; return *this; }
    constexpr Vector4T<T>& operator+=(T scalar) noexcept { position += scalar; w += scalar; return *this; }
    constexpr Vector4T<T>& operator-=(T scalar) noexcept { position -= scalar; w -= scalar; return *this; }
    constexpr Vector4T<T>& operator*=(T scalar) noexcept { position *= scalar; w *= scalar; return *this; }
    constexpr Vector4T<T>& operator/=(T scalar) noexcept { position /= scalar; w /= scalar; return *this; }
    // negation
    constexpr Vector4T<T> operator-() const noexcept { return {-position, -w}; }
    // comparison
    constexpr bool operator==(const Vector4T<T>& other) const noexcept { return position == other.position && w == other.w; }
    // dot product
    constexpr T dot(const Vector4T<T>& other) const noexcept { return position.dot(other.position) + w * other.w; }

    Vector3T<T> position;
    T w;
};

using Vector2 = Vector2T<float>;
using Vector3 = Vector3T<float>;
using Vector4 = Vector4T<float>;

using Vector2i = Vector2T<int32_t>;
using Vector3i = Vector3T<int32_t>;
using Vector4i = Vector4T<int32_t>;

class Vertex : public Vector4 {
public:
    constexpr Vertex() noexcept : Vector4(0.0f, 0.0f, 0.0f, 1.0f) {}
    constexpr Vertex(float x, float y, float z, float w) noexcept : Vector4(x, y, z, w) {}
    constexpr Vertex(const Vector3& other, float w) noexcept : Vector4(other, w) {}
    constexpr Vertex(const Vector3& other) noexcept : Vector4(other, 1.0f) {}
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
