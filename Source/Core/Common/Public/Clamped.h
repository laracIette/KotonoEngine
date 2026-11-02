#pragma once
#include <algorithm>
#include <limits>
#include <type_traits>

template <typename T>
concept Number = std::is_arithmetic_v<T>;

template <Number T, T Min = std::numeric_limits<T>::min(), T Max = std::numeric_limits<T>::max()>
	requires (Min < Max)
class UClamped final
{
private:
	using ValueType = T;

public:
	constexpr UClamped() : value_{ clamp(ValueType{}) } {}
	constexpr UClamped(ValueType value) : value_{ clamp(value) } {}
	
	constexpr ValueType Get() const noexcept
	{
		return value_;
	}

	constexpr operator ValueType() const noexcept
	{
		return Get();
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