#include "Task.h"
#include "TimeManager.h"

void UTask::Update(const float delta)
{
    if (!isPlaying)
    {
        return;
    }

    current_ += delta;

    if (current_ < duration)
    {
        eventUpdate.Broadcast();
    }
    else
    {
        Stop();
    }
    
}

void UTask::Start()
{
    isPlaying = true;
    current_ = 0.0f;
}

void UTask::Stop()
{
    isPlaying = false;
    eventCompleted.Broadcast();
}
