#include "Timer.h"

bool UTimer::GetIsPlaying() const
{
    return isPlaying_;
}

bool UTimer::GetIsRepeat() const
{
    return isRepeat_;
}

f32 UTimer::GetDuration() const
{
    return duration_;
}

UEvent<>& UTimer::EventCompleted()
{
    return eventCompleted_;
}

void UTimer::SetIsRepeat(const bool isRepeat)
{
    isRepeat_ = isRepeat;
}

void UTimer::SetDuration(const f32 duration)
{
    duration_ = duration;
}

void UTimer::Start(const bool isOverride)
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

void UTimer::Update(const f32 deltaTime)
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
        Start();
    }
    else
    {
        Stop();
    }
}
