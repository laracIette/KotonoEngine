#pragma once
#include <concepts>
#include <type_traits>

// Static cast
template <typename T, typename U>
    requires std::is_pointer_v<T*> && std::is_pointer_v<U*>
constexpr T* cast(U* inst) noexcept
{
    return static_cast<T*>(inst);
}

// Dynamic cast
template <typename T, typename U>
    requires std::is_pointer_v<T*> && std::is_pointer_v<U*>
inline T* try_cast(U* inst) noexcept
{
    return dynamic_cast<T*>(inst);
}