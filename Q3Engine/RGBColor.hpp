#pragma once

#include <cstdint>

namespace q3 {

class RGBColor {
public:
    constexpr RGBColor() : r(0), g(0), b(0) {}
    constexpr RGBColor(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {}

    uint8_t r, g, b;
};

}