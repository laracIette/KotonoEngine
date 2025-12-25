#pragma once
template <typename... Args>
class KtDelegate;
class KtStopwatch final
{
public:
    void Start();
    void Stop();

    float ElapsedSeconds() const;

    template <typename... Args>
    static float Time(const KtDelegate<Args...>& delegate, Args... args)
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
