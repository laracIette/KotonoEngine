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
	constexpr UClamped(ValueType value) : value_{ clamp(value) } {}

	constexpr operator ValueType() const noexcept
	{
		return value_;
	}

	static constexpr ValueType clamp(ValueType value) noexcept
	{
		return std::clamp(value, Min, Max);
	}

	static constexpr ValueType min() noexcept
	{
		return Min;
	}

	static constexpr ValueType max() noexcept
	{
		return Max;
	}

private:
	ValueType value_;
};