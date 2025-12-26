#pragma once
#include <cstdint>
enum class EAnchor : uint8_t
{
    Center = 0x00,
    Top = 0x01,
    Bottom = 0x02,
    Left = 0x04,
    Right = 0x08,
    TopLeft = Top | Left,
    TopRight = Top | Right,
    BottomLeft = Bottom | Left,
    BottomRight = Bottom | Right
};

inline constexpr EAnchor operator&(EAnchor first, EAnchor second)
{
    return static_cast<EAnchor>(static_cast<uint8_t>(first) & static_cast<uint8_t>(second));
}

inline constexpr EAnchor operator|(EAnchor first, EAnchor second)
{
    return static_cast<EAnchor>(static_cast<uint8_t>(first) | static_cast<uint8_t>(second));
}