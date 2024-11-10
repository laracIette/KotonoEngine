#include "Timer.h"

Timer::Timer() :
    _timeout([]() {}),
    _isTicking(false),
    _isPaused(false),
    _startTime(0.0f), 
    _pauseTime(0.0f), 
    _targetDuration(0.0f), 
    _currentDuration(0.0f), 
    _elapsedTime(0.0f), 
    _isLoop(false)
{
}

float Timer::GetTargetDuration() const
{
    return _targetDuration;
}

void Timer::SetTargetDuration(float targetDuration)
{
    _targetDuration = targetDuration;
}

bool Timer::GetIsLoop() const
{
    return _isLoop;
}

void Timer::SetIsLoop(bool isLoop)
{
    _isLoop = isLoop;
}

void Timer::SetTimeout(std::function<void()> timeout)
{
    _timeout = timeout;
}

void Timer::Update(float deltaTime)
{
    if (_isTicking && !_isPaused)
    {
        _elapsedTime += deltaTime;

        if (_elapsedTime >= _currentDuration)
        {
            _isTicking = false;

            if (_isLoop)
            {
                float overtime = _elapsedTime - _currentDuration;
                Reset(_targetDuration - overtime);
            }

            OnTimeout();
        }
    }
}

void Timer::Start()
{
    if (_targetDuration <= 0.0f)
    {
        throw "TargetDuration must be greater than 0";
    }

    Reset(_targetDuration);
}

void Timer::Pause()
{
    if (!_isTicking || _isPaused)
    {
        return;
    }

    _isPaused = true;
    _pauseTime = _elapsedTime;
}

void Timer::Resume()
{
    if (!_isTicking || !_isPaused)
    {
        return;
    }

    _isPaused = false;
    _startTime += _elapsedTime - _pauseTime; // Adjusting the start time
}

void Timer::Stop()
{
    _isTicking = false;
    _isPaused = false;
    _elapsedTime = 0.0f;
    _currentDuration = 0.0f;
}

void Timer::Reset(float duration)
{
    _isTicking = true;
    _isPaused = false;
    _elapsedTime = 0.0f;
    _currentDuration = duration;
}

void Timer::Switch()
{
    if (_isPaused)
    {
        Resume();
    }
    else
    {
        Pause();
    }
}

void Timer::OnTimeout() const
{
    _timeout();
}
