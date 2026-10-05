#include "Stopwatch/Stopwatch.h"

#include "Clock/Clock.h"

void UStopwatch::Start()
{
    start_ = UClock::ExactUTC();
}

void UStopwatch::Stop()
{
    end_ = UClock::ExactUTC();
}
