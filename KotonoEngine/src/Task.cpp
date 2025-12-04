#include "Task.h"
#include "Engine.h"
#include "TimeManager.h"

void UTask::Update()
{
    if (!isPlaying)
    {
        return;
    }

    current_ += Engine.TimeManager().GameTime().lastDelta;

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
