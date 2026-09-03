#include "Task.h"

void UTask::Update(f32 deltaTime)
{
    if (!isPlaying)
    {
        return;
    }

    current_ += deltaTime;

    if (current_ < duration)
    {
        eventUpdate.Broadcast(deltaTime);
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
