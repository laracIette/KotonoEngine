#include "Task.h"
#include <kotono_framework/Framework.h>

void OTask::Init()
{
    //_startTime = Framework.GetTime().GetNow();
}

void OTask::Update()
{
    
    if (_isPlaying)
    {
        //if (Framework.GetTime().GetNow() - _startTime < _duration)
        if (true)
        {
            _updateEvent.Broadcast();
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

KtEvent& OTask::GetEventUpdate()
{
    return _updateEvent;
}

void OTask::SetDuration(const float duration)
{
    _duration = duration;
}

void OTask::Play()
{
    _isPlaying = true;
}

void OTask::Stop()
{
    _isPlaying = false;
    //SetIsDelete(true);
}
