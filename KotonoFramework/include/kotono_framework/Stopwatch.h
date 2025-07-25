#pragma once
#include <chrono>
#include <concepts>
#include "Delegate.h"

class KtStopwatch final
{
private:
    using Clock = std::chrono::high_resolution_clock;
    using TimePoint = Clock::time_point;

public:
    void Start()
    {
        start_ = Clock::now();
    }

    void Stop()
    {
        end_ = Clock::now();
    }

    template <std::floating_point T>
    const T GetElapsedSeconds() const
    {
        return std::chrono::duration<T>(end_ - start_).count();
    }

    const int64_t GetElapsedMicroseconds() const
    {
        return std::chrono::duration_cast<std::chrono::microseconds>(end_ - start_).count();
    }

    const int64_t GetElapsedNanoseconds() const
    {
        return std::chrono::duration_cast<std::chrono::nanoseconds>(end_ - start_).count();
    }

    template <std::floating_point T>
    static const T Time(const KtDelegate<>& delegate)
    {
        KtStopwatch stopwatch{};
        stopwatch.Start();
        delegate.Callback();
        stopwatch.Stop();
        return stopwatch.GetElapsedSeconds<T>();
    }

private:
    TimePoint start_;
    TimePoint end_;
};