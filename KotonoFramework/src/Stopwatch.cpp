#include "Stopwatch.h"
#include "Framework.h"
#include "Clock.h"

void KtStopwatch::Start()
{
    start_ = Framework.Clock().Now();
}

void KtStopwatch::Stop()
{
    end_ = Framework.Clock().Now();
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
