#include "Task.h"
#include "Engine.h"
#include "Time.h"

void KTask::Update()
{
    Base::Update();

    if (_isPlaying)
    {
        if (Engine.GetTime().GetNow() - _startTime < _duration)
        {
            _eventUpdate.Broadcast();
        }
        else
        {
            Stop();
        }
    }
}

const float KTask::GetDuration() const
{
    return _duration;
}

KtEvent<>& KTask::GetEventUpdate()
{
    return _eventUpdate;
}

KtEvent<>& KTask::GetEventCompleted()
{
    return _eventCompleted;
}

void KTask::SetDuration(const float duration)
{
    _duration = duration;
}

void KTask::Start()
{
    _isPlaying = true;
    _startTime = Engine.GetTime().GetNow();
}

void KTask::Stop()
{
    _isPlaying = false;
    _eventCompleted.Broadcast();
}
