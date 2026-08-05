#pragma once
#include <functional>
#include <kotono_common/types.h>
class UStopwatch final
{
public:
    using TimeFunction = std::function<void()>;

public:
    void Start();
    void Stop();

    f32 ElapsedSeconds() const;

    static f32 Time(const TimeFunction& timeFunction);

private:
    f32 start_;
    f32 end_;
};
