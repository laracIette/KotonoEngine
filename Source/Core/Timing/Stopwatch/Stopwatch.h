#pragma once
#include <concepts>
#include <functional>
#include <kotono_common/types.h>
class UStopwatch final
{
public:
    using TimeFunction = std::function<void()>;

public:
    void Start();
    void Stop();

    template <std::floating_point T>
    auto ElapsedSeconds() const -> T { return static_cast<T>(end_ - start_); }
    
    template <std::floating_point T>
    static auto Time(TimeFunction const& timeFunction) -> T
    {
        UStopwatch stopwatch{};
        stopwatch.Start();
        timeFunction();
        stopwatch.Stop();
        return stopwatch.ElapsedSeconds<T>();
    }

private:
    f64 start_;
    f64 end_;
};
