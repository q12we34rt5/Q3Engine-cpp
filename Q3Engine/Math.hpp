#pragma once

namespace q3 {

class Vector3 {
public:
    constexpr Vector3() : x(0), y(0), z(0) {}
    constexpr Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

    // +, -, *, /
    constexpr Vector3 operator+(const Vector3& other) const noexcept { return {x + other.x, y + other.y, z + other.z}; }
    constexpr Vector3 operator-(const Vector3& other) const noexcept { return {x - other.x, y - other.y, z - other.z}; }
    constexpr Vector3 operator*(const Vector3& other) const noexcept { return {x * other.x, y * other.y, z * other.z}; }
    constexpr Vector3 operator/(const Vector3& other) const noexcept { return {x / other.x, y / other.y, z / other.z}; }
    constexpr Vector3 operator+(float scalar) const noexcept { return {x + scalar, y + scalar, z + scalar}; }
    constexpr Vector3 operator-(float scalar) const noexcept { return {x - scalar, y - scalar, z - scalar}; }
    constexpr Vector3 operator*(float scalar) const noexcept { return {x * scalar, y * scalar, z * scalar}; }
    constexpr Vector3 operator/(float scalar) const noexcept { return {x / scalar, y / scalar, z / scalar}; }
    // +=, -=, *=, /=
    constexpr Vector3& operator+=(const Vector3& other) noexcept { x += other.x; y += other.y; z += other.z; return *this; }
    constexpr Vector3& operator-=(const Vector3& other) noexcept { x -= other.x; y -= other.y; z -= other.z; return *this; }
    constexpr Vector3& operator*=(const Vector3& other) noexcept { x *= other.x; y *= other.y; z *= other.z; return *this; }
    constexpr Vector3& operator/=(const Vector3& other) noexcept { x /= other.x; y /= other.y; z /= other.z; return *this; }
    constexpr Vector3& operator+=(float scalar) noexcept { x += scalar; y += scalar; z += scalar; return *this; }
    constexpr Vector3& operator-=(float scalar) noexcept { x -= scalar; y -= scalar; z -= scalar; return *this; }
    constexpr Vector3& operator*=(float scalar) noexcept { x *= scalar; y *= scalar; z *= scalar; return *this; }
    constexpr Vector3& operator/=(float scalar) noexcept { x /= scalar; y /= scalar; z /= scalar; return *this; }
    // negation
    constexpr Vector3 operator-() const noexcept { return {-x, -y, -z}; }
    // comparison
    constexpr bool operator==(const Vector3& other) const noexcept { return x == other.x && y == other.y && z == other.z; }
    // dot product
    constexpr float dot(const Vector3& other) const noexcept { return x * other.x + y * other.y + z * other.z; }

    float x, y, z;
};

class Vector2 {
public:
    constexpr Vector2() : x(0), y(0) {}
    constexpr Vector2(float x, float y) : x(x), y(y) {}
    // from Vector3
    constexpr explicit Vector2(const Vector3& other) : x(other.x), y(other.y) {}

    // +, -, *, /
    constexpr Vector2 operator+(const Vector2& other) const noexcept { return {x + other.x, y + other.y}; }
    constexpr Vector2 operator-(const Vector2& other) const noexcept { return {x - other.x, y - other.y}; }
    constexpr Vector2 operator*(const Vector2& other) const noexcept { return {x * other.x, y * other.y}; }
    constexpr Vector2 operator/(const Vector2& other) const noexcept { return {x / other.x, y / other.y}; }
    constexpr Vector2 operator+(float scalar) const noexcept { return {x + scalar, y + scalar}; }
    constexpr Vector2 operator-(float scalar) const noexcept { return {x - scalar, y - scalar}; }
    constexpr Vector2 operator*(float scalar) const noexcept { return {x * scalar, y * scalar}; }
    constexpr Vector2 operator/(float scalar) const noexcept { return {x / scalar, y / scalar}; }
    // +=, -=, *=, /=
    constexpr Vector2& operator+=(const Vector2& other) noexcept { x += other.x; y += other.y; return *this; }
    constexpr Vector2& operator-=(const Vector2& other) noexcept { x -= other.x; y -= other.y; return *this; }
    constexpr Vector2& operator*=(const Vector2& other) noexcept { x *= other.x; y *= other.y; return *this; }
    constexpr Vector2& operator/=(const Vector2& other) noexcept { x /= other.x; y /= other.y; return *this; }
    constexpr Vector2& operator+=(float scalar) noexcept { x += scalar; y += scalar; return *this; }
    constexpr Vector2& operator-=(float scalar) noexcept { x -= scalar; y -= scalar; return *this; }
    constexpr Vector2& operator*=(float scalar) noexcept { x *= scalar; y *= scalar; return *this; }
    constexpr Vector2& operator/=(float scalar) noexcept { x /= scalar; y /= scalar; return *this; }
    // negation
    constexpr Vector2 operator-() const noexcept { return {-x, -y}; }
    // comparison
    constexpr bool operator==(const Vector2& other) const noexcept { return x == other.x && y == other.y; }
    // dot product
    constexpr float dot(const Vector2& other) const noexcept { return x * other.x + y * other.y; }

    float x, y;
};

class Vertex {
public:
    Vertex() : position(), w(1) {}
    Vertex(const Vector3& position) : position(position), w(1) {}
    Vertex(const Vector3& position, float w) : position(position), w(w) {}
    Vertex(float x, float y, float z) : position(x, y, z), w(1) {}
    Vertex(float x, float y, float z, float w) : position(x, y, z), w(w) {}

    Vector3 position;
    float w;
};

}
