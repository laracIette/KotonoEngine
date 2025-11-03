#include "Task.h"
#include "Engine.h"
#include "TimeManager.h"

void KTask::Init()
{
    SetCanUpdate(true);
}

void KTask::Update()
{
    Base::Update();

    if (!isPlaying_)
    {
        return;
    }

    current_ += current_.IsSeconds()
        ? UDuration::FromSeconds(Engine.TimeManager().GameTime().lastDelta)
        : UDuration::FromUpdates(1);

    if (current_ < duration_)
    {
        eventUpdate_.Broadcast();
    }
    else
    {
        Stop();
    }
    
}

const UDuration& KTask::GetDuration() const
{
    return duration_;
}

KtEvent<>& KTask::EventUpdate()
{
    return eventUpdate_;
}

KtEvent<>& KTask::EventCompleted()
{
    return eventCompleted_;
}

void KTask::SetDuration(const UDuration& duration)
{
    duration_ = duration;
}

void KTask::Start()
{
    isPlaying_ = true;
    current_ = current_.IsSeconds()
        ? UDuration::FromSeconds(0.0f)
        : UDuration::FromUpdates(0);
}

void KTask::Stop()
{
    isPlaying_ = false;
    eventCompleted_.Broadcast();
}
