#pragma once
#include "Delegate.h"

class KtStopwatch final
{
public:
    void Start();
    void Stop();

    float ElapsedSeconds() const;

    static float Time(const KtDelegate<>& delegate);

private:
    float start_;
    float end_;
};
