#include "Timer.h"

void UTimer::Start(b8 isOverride)
{
    if (isPlaying_ && !isOverride)
    {
        return;
    }

    isPlaying_ = true;
    current_ = 0.0f;
}

void UTimer::Stop()
{
    isPlaying_ = false;
}

void UTimer::Update(f32 deltaTime)
{
    if (!isPlaying_)
    {
        return;
    }

    current_ += deltaTime;

    if (current_ < duration_)
    {
        return;
    }

    eventCompleted_.Broadcast();
    if (isRepeat_)
    {
        current_ -= duration_;
    }
    else
    {
        Stop();
    }
}
