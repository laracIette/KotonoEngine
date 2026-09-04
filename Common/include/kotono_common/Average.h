#pragma once
#include "types.h"
#include <array>
#include <concepts>
template <std::floating_point T, size Size>
class UAverage final
{
public:
	constexpr UAverage() : values_{}, index_ { 0 }, sum_{ 0 } {}

	constexpr void Add(T time) noexcept
	{
		sum_ -= values_[index_];
		values_[index_] = time;
		sum_ += values_[index_];
		index_ = (index_ + 1) % values_.size();
	}

	constexpr T Get() const noexcept
	{
		return sum_ / values_.size();
	}

private:
	std::array<T, Size> values_;
	size index_;
	T sum_;
};

