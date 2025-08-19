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

    const UDuration elapsed{ GetNow() - start_ };
    if (elapsed < currentDuration_)
    {
        return;
    }

    eventCompleted_.Broadcast();
    if (isRepeat_)
    {
        const UDuration overtime{ elapsed - currentDuration_ };
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

const UDuration& KTimer::GetDuration() const
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

void KTimer::SetDuration(const UDuration& duration)
{
    targetDuration_ = duration;
    currentDuration_ = duration;
}

void KTimer::Start(const bool isOverride)
{
    if (isPlaying_ && !isOverride)
    {
        return;
    }

    isPlaying_ = true;

    start_ = GetNow();
}

void KTimer::Stop()
{
    isPlaying_ = false;
}

UDuration KTimer::GetNow() const
{
    return std::holds_alternative<float>(targetDuration_.value)
        ? UDuration::FromSeconds(Engine.GetTimeManager().GetNow())
        : UDuration::FromUpdates(Engine.GetObjectManager().GetCurrentUpdate());
}
