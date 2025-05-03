#include "Task.h"
#include "Engine.h"
#include "Time.h"

void OTask::Update()
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

const float OTask::GetDuration() const
{
    return _duration;
}

KtEvent<>& OTask::GetEventUpdate()
{
    return _eventUpdate;
}

KtEvent<>& OTask::GetEventCompleted()
{
    return _eventCompleted;
}

void OTask::SetDuration(const float duration)
{
    _duration = duration;
}

void OTask::Start()
{
    _isPlaying = true;
    _startTime = Engine.GetTime().GetNow();
}

void OTask::Stop()
{
    _isPlaying = false;
    _eventCompleted.Broadcast();
}
