#include "Timer.h"
#include "TimeManager.h"

void UTimer::Update()
{
    if (!isPlaying)
    {
        return;
    }

    current_ += TimeManager.GameTime().lastDelta;

    if (current_ < duration)
    {
        return;
    }

    eventCompleted.Broadcast();
    if (isRepeat)
    {
        current_ -= duration;
        Start();
    }
    else
    {
        Stop();
    }
}

void UTimer::Start(const bool isOverride)
{
    if (isPlaying && !isOverride)
    {
        return;
    }

    isPlaying = true;
    current_ = 0.0f;
}

void UTimer::Stop()
{
    isPlaying = false;
}
