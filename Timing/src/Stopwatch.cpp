#include "Stopwatch.h"

#include "Clock.h"

void UStopwatch::Start()
{
    start_ = UClock::ExactUTC();
}

void UStopwatch::Stop()
{
    end_ = UClock::ExactUTC();
}
