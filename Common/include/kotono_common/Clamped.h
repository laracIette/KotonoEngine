#pragma once
#include <algorithm>
#include <type_traits>
template <typename T>
concept Number = std::is_arithmetic_v<T>;

template <Number T, T Min, T Max>
	requires (Min < Max)
class UClamped final
{
private:
	using ValueType = T;

public:
	constexpr UClamped(ValueType value) : value_{ Clamp(value) } {}

	constexpr operator ValueType() const noexcept
	{
		return value_;
	}

private:
	static constexpr ValueType Clamp(ValueType value) noexcept
	{
		return std::clamp(value, Min, Max);
	}

private:
	ValueType value_;
};