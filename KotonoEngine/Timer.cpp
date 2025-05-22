#include "Timer.h"
#include "Engine.h"
#include "Time.h"

void KTimer::Update()
{
	Base::Update();

    if (!isPlaying_)
    {
        return;
    }

    const float elapsedTime = Engine.GetTime().GetNow() - startTime_;
    
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

const bool KTimer::GetIsRepeat() const
{
    return isRepeat_;
}

const float KTimer::GetDuration() const
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

void KTimer::Start()
{
    isPlaying_ = true;
    startTime_ = Engine.GetTime().GetNow();
}

void KTimer::Stop()
{
    isPlaying_ = false;
}