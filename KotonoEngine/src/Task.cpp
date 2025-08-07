#include "Task.h"
#include "Engine.h"
#include "TimeManager.h"

void KTask::Update()
{
    Base::Update();

    if (isPlaying_)
    {
        if (Engine.GetTimeManager().GetNow() - startTime_ < duration_)
        {
            eventUpdate_.Broadcast();
        }
        else
        {
            Stop();
        }
    }
}

float KTask::GetDuration() const
{
    return duration_;
}

KtEvent<>& KTask::GetEventUpdate()
{
    return eventUpdate_;
}

KtEvent<>& KTask::GetEventCompleted()
{
    return eventCompleted_;
}

void KTask::SetDuration(const float duration)
{
    duration_ = duration;
}

void KTask::Start()
{
    isPlaying_ = true;
    startTime_ = Engine.GetTimeManager().GetNow();
}

void KTask::Stop()
{
    isPlaying_ = false;
    eventCompleted_.Broadcast();
}
