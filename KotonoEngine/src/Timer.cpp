#include "Timer.h"
#include "Engine.h"
#include "TimeManager.h"

void KTimer::Update()
{
	Base::Update();

    if (!isPlaying_)
    {
        return;
    }

    const float elapsedTime = Engine.GetTimeManager().GetNow() - startTime_;
    
    if (elapsedTime < currentDuration_)
    {
        return;
    }

    eventCompleted_.Broadcast();
    if (isRepeat_)
    {
        const float overtime = elapsedTime - currentDuration_;
        currentDuration_ = targetDuration_ - overtime;
        Start();
    }
    else
    {
        Stop();
    }
}

bool KTimer::GetIsPlaying() const
{
    return isPlaying_;
}

bool KTimer::GetIsRepeat() const
{
    return isRepeat_;
}

float KTimer::GetDuration() const
{
    return targetDuration_;
}

KtEvent<>& KTimer::GetEventCompleted()
{
    return eventCompleted_;
}

void KTimer::SetIsRepeat(const bool isRepeat)
{
    isRepeat_ = isRepeat;
}

void KTimer::SetDuration(const float duration)
{
    targetDuration_ = duration;
    currentDuration_ = targetDuration_;
}

void KTimer::Start(const bool isOverride)
{
    if (!isOverride && isPlaying_)
    {
        return;
    }

    isPlaying_ = true;
    startTime_ = Engine.GetTimeManager().GetNow();
}

void KTimer::Stop()
{
    isPlaying_ = false;
}