#include "Timer.h"

void KtTimer::Update(const float delta)
{
    if (!isPlaying_)
    {
        return;
    }

    current_ += delta;

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

bool KtTimer::GetIsPlaying() const
{
    return isPlaying_;
}

bool KtTimer::GetIsRepeat() const
{
    return isRepeat_;
}

float KtTimer::GetDuration() const
{
    return duration_;
}

KtEvent<>& KtTimer::EventCompleted()
{
    return eventCompleted_;
}

void KtTimer::SetIsRepeat(const bool isRepeat)
{
    isRepeat_ = isRepeat;
}

void KtTimer::SetDuration(const float duration)
{
    duration_ = duration;
}

void KtTimer::Start(const bool isOverride)
{
    if (isPlaying_ && !isOverride)
    {
        return;
    }

    isPlaying_ = true;
    current_ = 0.0f;
}

void KtTimer::Stop()
{
    isPlaying_ = false;
}
