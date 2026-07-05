#include "Clock.h"
#include <chrono>

f64 SClock::startUTC_{ SClock::ExactUTC() };

f64 SClock::ExactUTC()
{
	const auto now{ std::chrono::system_clock::now() };
	const auto duration{ now.time_since_epoch() };
	return std::chrono::duration<f64>{ duration }.count();
}

f64 SClock::StartUTC()
{
	return startUTC_;
}

f32 SClock::Now()
{
	return static_cast<f32>(ExactUTC() - startUTC_);
}
