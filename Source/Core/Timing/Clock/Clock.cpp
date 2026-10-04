#include "Clock.h"
#include <chrono>

void UClock::Init()
{
	startUTC_ = ExactUTC();
	nowUTC_ = startUTC_;
}

void UClock::Update()
{
	nowUTC_ = ExactUTC();
}

f64 UClock::ExactUTC()
{
	const auto now{ std::chrono::system_clock::now() };
	const auto duration{ now.time_since_epoch() };
	return std::chrono::duration<f64>{ duration }.count();
}
