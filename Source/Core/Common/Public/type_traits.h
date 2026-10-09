#pragma once
#include "types.h"
#include <type_traits>

template <typename T>
struct is_cheap_copyable final : std::bool_constant<std::is_trivially_copyable_v<std::remove_cvref_t<T>> && sizeof(std::remove_cvref_t<T>) <= 16>
{
};

template <typename T>
inline constexpr b8 is_cheap_copyable_v = is_cheap_copyable<T>::value;

template <typename T>
using optimal_t = std::conditional_t<
	is_cheap_copyable_v<T>,
	std::remove_cvref_t<T>,
	std::remove_cvref_t<T> const&
>;
