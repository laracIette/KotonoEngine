#pragma once
#include <array>

template <size_t Size>
class KtAverageTime final
{
public:
	void AddTime(const float time)
	{
		timesSum_ -= times_[timeIndex_];
		times_[timeIndex_] = time;
		timesSum_ += times_[timeIndex_];
		timeIndex_ = (timeIndex_ + 1) % times_.size();
	}

	float GetAverageTime() const
	{
		return timesSum_ / times_.size();
	}

private:
	std::array<float, Size> times_;
	size_t timeIndex_;
	float timesSum_;
};

