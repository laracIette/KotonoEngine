#include "Stopwatch.h"

void KtStopwatch::Start()
{
    start_ = Clock::now();
}

void KtStopwatch::Stop()
{
    end_ = Clock::now();
}

const double KtStopwatch::GetElapsedSeconds() const
{
    return std::chrono::duration<double>(end_ - start_).count();
}

const int64_t KtStopwatch::GetElapsedMicroseconds() const
{
    return std::chrono::duration_cast<std::chrono::microseconds>(end_ - start_).count();
}

const int64_t KtStopwatch::GetElapsedNanoseconds() const
{
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end_ - start_).count();
}
