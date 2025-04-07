#pragma once
enum class EAnchor : int
{
    Center = 0x00000000,
    Top = 0x00000001,
    Bottom = 0x00000002,
    Left = 0x00000004,
    Right = 0x00000008,
    TopLeft = Top | Left,
    TopRight = Top | Right,
    BottomLeft = Bottom | Left,
    BottomRight = Bottom | Right
};

inline constexpr EAnchor operator&(EAnchor first, EAnchor second)
{
    return static_cast<EAnchor>(static_cast<int>(first) & static_cast<int>(second));
}

inline constexpr EAnchor operator|(EAnchor first, EAnchor second)
{
    return static_cast<EAnchor>(static_cast<int>(first) | static_cast<int>(second));
}