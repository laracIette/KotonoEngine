#include "Clock.h"
#include <chrono>

double KtClock::startUTC_{ KtClock::ExactUTC() };

double KtClock::ExactUTC()
{
	const auto now{ std::chrono::system_clock::now() };
	const auto duration{ now.time_since_epoch() };
	return std::chrono::duration<double>(duration).count();
}

float KtClock::Now()
{
	return static_cast<float>(ExactUTC() - startUTC_);
}
