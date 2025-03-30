#include "Time.h"
#include <chrono>

void KTime::Init()
{
    _startUTC = GetExactUTC();
}

void KTime::Update()
{
    double currentUTC = GetExactUTC();
    _nowTime = static_cast<float>(currentUTC - _startUTC);

    _deltaTime = _nowTime - _previousTime;
    _previousTime = _nowTime;
}

void KTime::Cleanup()
{
}

const double KTime::GetExactUTC() const
{
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    return std::chrono::duration<double>(duration).count();
}

const float KTime::GetNow() const
{
    return _nowTime;
}

const float KTime::GetDelta() const
{
    return _deltaTime;
}
