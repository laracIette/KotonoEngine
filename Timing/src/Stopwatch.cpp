#include "Stopwatch.h"
#include "Clock.h"
#include <kotono_common/Delegate.h>

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

float KtStopwatch::Time(const KtDelegate<>& delegate)
{
    KtStopwatch stopwatch{};
    stopwatch.Start();
    delegate.Callback();
    stopwatch.Stop();
    return stopwatch.ElapsedSeconds();
}
