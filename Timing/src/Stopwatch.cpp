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
