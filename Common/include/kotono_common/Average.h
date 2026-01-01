#pragma once
#include <array>
#include "types.h"
template <size Size = 64, std::floating_point T = float>
class KtAverage final
{
public:
	constexpr void Add(const T time) noexcept
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

