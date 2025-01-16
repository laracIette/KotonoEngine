#pragma once
#include "Updatable.h"
#include <functional>

class Timer final :
    public Updatable
{
    BASECLASS(Updatable)

public:
    Timer();

    float GetTargetDuration() const;
    void SetTargetDuration(float targetDuration);

    bool GetIsLoop() const;
    void SetIsLoop(bool isLoop);

    void SetTimeout(std::function<void()> timeout);
    
    void Update(float deltaTime) override;

    // Start the timer
    void Start();

    // Pause the timer
    void Pause();

    // Resume the timer
    void Resume();

    // Stop the timer
    void Stop();

    // Switch between pause and resume
    void Switch();

private:
    bool _isTicking;
    bool _isPaused;

    float _startTime;
    float _pauseTime;

    float _targetDuration;
    float _currentDuration;

    float _elapsedTime;

    bool _isLoop;

    void Reset(float duration);

    std::function<void()> _timeout;

    void OnTimeout() const;
};


