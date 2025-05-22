#include "Task.h"
#include "Engine.h"
#include "Time.h"

void KTask::Update()
{
    Base::Update();

    if (isPlaying_)
    {
        if (Engine.GetTime().GetNow() - startTime_ < duration_)
        {
            eventUpdate_.Broadcast();
        }
        else
        {
            Stop();
        }
    }
}

const float KTask::GetDuration() const
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
    startTime_ = Engine.GetTime().GetNow();
}

void KTask::Stop()
{
    isPlaying_ = false;
    eventCompleted_.Broadcast();
}
