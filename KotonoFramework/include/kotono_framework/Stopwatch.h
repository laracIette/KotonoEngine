#pragma once
#include <chrono>

class KtStopwatch final
{
private:
    using Clock = std::chrono::high_resolution_clock;
    using TimePoint = Clock::time_point;

public:
    void Start();
    void Stop();

    const double GetElapsedSeconds() const;
    const int64_t GetElapsedMicroseconds() const;
    const int64_t GetElapsedNanoseconds() const;

private:
    TimePoint start_;
    TimePoint end_;
};