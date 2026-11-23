#pragma once
#include <type_traits>

template <typename>
struct IsCopyable final : std::false_type {};

template <typename T>
inline constexpr auto VIsCopyable{ IsCopyable<T>::value };

template <typename T>
struct IsCopyableOrCheap final : std::bool_constant<
    VIsCopyable<T> ||
    (std::is_trivially_copyable_v<std::remove_cvref_t<T>> && sizeof(std::remove_cvref_t<T>) <= 8)
> {};

template <typename T>
inline constexpr auto VIsCopyableOrCheap{ IsCopyableOrCheap<T>::value };

template <typename T>
struct Optimal final : std::conditional<
    VIsCopyableOrCheap<T>,
    std::remove_cvref_t<T>,
    std::remove_cvref_t<T> const&
> {};

template <typename T>
using TOptimal = Optimal<T>::type;

template <typename Base, typename Derived>
struct IsStrictBaseOf final : std::bool_constant<
    !std::is_same_v<Base, Derived> &&
    std::is_base_of_v<Base, Derived>
> {};

template <typename Base, typename Derived>
inline constexpr auto VIsStrictBaseOf{ IsStrictBaseOf<Base, Derived>::value };

template <typename T>
[[nodiscard]] constexpr std::remove_reference_t<T>&& Move(T&& Value) noexcept
{
    return static_cast<std::remove_reference_t<T>&&>(Value);
}
