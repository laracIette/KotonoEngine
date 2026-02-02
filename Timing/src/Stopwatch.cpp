#include "Stopwatch.h"
#include "Clock.h"

void KtStopwatch::Start()
{
    start_ = KtClock::Now();
}

void KtStopwatch::Stop()
{
    end_ = KtClock::Now();
}

float KtStopwatch::ElapsedSeconds() const
{
    return end_ - start_;
}

float KtStopwatch::Time(const TimeFunction& timeFunction)
{
    KtStopwatch stopwatch{};
    stopwatch.Start();
    timeFunction();
    stopwatch.Stop();
    return stopwatch.ElapsedSeconds();
}