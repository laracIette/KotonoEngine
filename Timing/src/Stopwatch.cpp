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

float UStopwatch::ElapsedSeconds() const
{
    return end_ - start_;
}

float UStopwatch::Time(const TimeFunction& timeFunction)
{
    UStopwatch stopwatch{};
    stopwatch.Start();
    timeFunction();
    stopwatch.Stop();
    return stopwatch.ElapsedSeconds();
}