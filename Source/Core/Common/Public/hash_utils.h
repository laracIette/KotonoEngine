#pragma once
#include "types.h"
#include <string_view>

constexpr size GLD_RATIO{ 0x9e3779b97f4a7c15ULL };

constexpr size FNV_OFFSET_BASIS{ 0xcbf29ce484222325ULL };
constexpr size FNV_PRIME{ 0x100000001b3ULL };

[[nodiscard]] constexpr size combine(const size h, const size v) noexcept
{
    return h ^ (v + GLD_RATIO + (h << 6) + (h >> 2));
}

[[nodiscard]] consteval size ce_combine(const size h, const size v) noexcept
{
    return combine(h, v);
}

[[nodiscard]] constexpr size hash_str(std::string_view str) noexcept
{
    size hash{ FNV_OFFSET_BASIS };

    for (const char c : str)
    {
        hash ^= static_cast<size>(c);
        hash *= FNV_PRIME;
    }

    return hash;
}

[[nodiscard]] consteval size ce_hash_str(std::string_view str) noexcept
{
    return hash_str(str);
}

[[nodiscard]] inline size hash_ptr(const void* ptr) noexcept
{
    size x{ static_cast<size>(reinterpret_cast<std::uintptr_t>(ptr)) };

    x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
    x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
    x = x ^ (x >> 31);

    return x;
}
