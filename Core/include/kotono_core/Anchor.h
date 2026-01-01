#pragma once
#include <kotono_common/types.h>
enum class EAnchor : u8
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
    return static_cast<EAnchor>(static_cast<u8>(first) & static_cast<u8>(second));
}

inline constexpr EAnchor operator|(EAnchor first, EAnchor second)
{
    return static_cast<EAnchor>(static_cast<u8>(first) | static_cast<u8>(second));
}