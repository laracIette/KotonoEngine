#pragma once
#include <functional>
class UStopwatch final
{
public:
    using TimeFunction = std::function<void()>;

public:
    void Start();
    void Stop();

    float ElapsedSeconds() const;

    static float Time(const TimeFunction& timeFunction);

private:
    float start_;
    float end_;
};
