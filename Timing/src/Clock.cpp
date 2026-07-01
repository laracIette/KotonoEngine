#include "Clock.h"
#include <chrono>

double SClock::startUTC_{ SClock::ExactUTC() };

double SClock::ExactUTC()
{
	const auto now{ std::chrono::system_clock::now() };
	const auto duration{ now.time_since_epoch() };
	return std::chrono::duration<double>(duration).count();
}

float SClock::Now()
{
	return static_cast<float>(ExactUTC() - startUTC_);
}
