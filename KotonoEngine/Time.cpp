#include "Time.h"
#include <chrono>

void STime::Init()
{
    _startUTC = GetExactUTC();
}

void STime::Update()
{
    double currentUTC = GetExactUTC();
    _nowTime = static_cast<float>(currentUTC - _startUTC);

    _deltaTime = _nowTime - _previousTime;
    _previousTime = _nowTime;
}

void STime::Cleanup()
{
}

const double STime::GetExactUTC() const
{
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    return std::chrono::duration<double>(duration).count();
}

const float STime::GetNow() const
{
    return _nowTime;
}

const float STime::GetDelta() const
{
    return _deltaTime;
}
