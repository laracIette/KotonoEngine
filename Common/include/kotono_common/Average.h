#pragma once
#include "types.h"
#include <array>
#include <concepts>
#include <numeric>
#include <utility>
template <std::floating_point T, size Size>
class UAverage final
{
	static_assert(Size > 0, "Size must be greater than 0");

public:
	constexpr UAverage() : values_{}, index_{ 0 }, count_{ 0 }, sum_{ 0 } {}

	constexpr void Add(T time) noexcept
	{
		sum_ -= values_[index_];
		values_[index_] = time;
		sum_ += values_[index_];
		index_ = (index_ + 1) % Size;

		count_ = std::min(count_ + 1, Size);

		if (index_ == 0)
		{
			sum_ = std::accumulate(values_.begin(), values_.begin() + count_, T{ 0 });
		}
	}

	constexpr T Get() const noexcept
	{
		if (count_ == 0) [[unlikely]]
		{
			return T{ 0 };
		}
		return sum_ / static_cast<T>(count_);
	}

private:
	std::array<T, Size> values_;
	size index_;
	size count_;
	T sum_;
};

