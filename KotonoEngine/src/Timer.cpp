#include "Timer.h"
#include "Engine.h"
#include "TimeManager.h"
#include "ObjectManager.h"
#include "log.h"

void KTimer::Init()
{
    SetCanUpdate(true);
}

void KTimer::Update()
{
	Base::Update();

    if (!isPlaying_)
    {
        return;
    }

    current_ += duration_.IsSeconds()
        ? UDuration::FromSeconds(Engine.TimeManager().GameTime().lastDelta)
        : UDuration::FromUpdates(1);

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
    return duration_;
}

KtEvent<>& KTimer::EventCompleted()
{
    return eventCompleted_;
}

void KTimer::SetIsRepeat(const bool isRepeat)
{
    isRepeat_ = isRepeat;
}

void KTimer::SetDuration(const UDuration& duration)
{
    duration_ = duration;
}

void KTimer::Start(const bool isOverride)
{
    if (isPlaying_ && !isOverride)
    {
        return;
    }

    isPlaying_ = true;

    current_ = duration_.IsSeconds()
        ? UDuration::FromSeconds(0.0f)
        : UDuration::FromUpdates(0);
}

void KTimer::Stop()
{
    isPlaying_ = false;
}
