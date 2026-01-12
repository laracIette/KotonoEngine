#pragma once
template <typename... Args>
class UDelegate;
class KtStopwatch final
{
public:
    void Start();
    void Stop();

    float ElapsedSeconds() const;

    template <typename... Args>
    static float Time(const UDelegate<Args...>& delegate, Args... args)
    {
        KtStopwatch stopwatch{};
        stopwatch.Start();
        delegate.Callback(args...);
        stopwatch.Stop();
        return stopwatch.ElapsedSeconds();
    }

private:
    float start_;
    float end_;
};
