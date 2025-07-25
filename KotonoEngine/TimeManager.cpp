#include "TimeManager.h"
#include <chrono>

void STimeManager::Init()
{
    _startUTC = GetExactUTC();
}

void STimeManager::Update()
{
    double currentUTC = GetExactUTC();
    _nowTime = static_cast<float>(currentUTC - _startUTC);

    _deltaTime = _nowTime - _previousTime;
    _previousTime = _nowTime;
}

void STimeManager::Cleanup()
{
}

const double STimeManager::GetExactUTC() const
{
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    return std::chrono::duration<double>(duration).count();
}

const float STimeManager::GetNow() const
{
    return _nowTime;
}

const float STimeManager::GetDelta() const
{
    return _deltaTime;
}
