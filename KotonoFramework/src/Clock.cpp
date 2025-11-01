#include "Clock.h"
#include <chrono>

void KtClock::Init()
{
	_startUTC = ExactUTC();
}

double KtClock::ExactUTC() const
{
	const auto now{ std::chrono::system_clock::now() };
	const auto duration{ now.time_since_epoch() };
	return std::chrono::duration<double>(duration).count();
}

float KtClock::Now() const
{
	return static_cast<float>(ExactUTC() - _startUTC);
}
