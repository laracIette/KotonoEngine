#include "Stopwatch.h"
#include "Clock.h"

void UStopwatch::Start()
{
    start_ = SClock::Now();
}

void UStopwatch::Stop()
{
    end_ = SClock::Now();
}

f32 UStopwatch::ElapsedSeconds() const
{
    return end_ - start_;
}

f32 UStopwatch::Time(const TimeFunction& timeFunction)
{
    UStopwatch stopwatch{};
    stopwatch.Start();
    timeFunction();
    stopwatch.Stop();
    return stopwatch.ElapsedSeconds();
}