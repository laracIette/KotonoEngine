#include "TimeManager.h"
#include "Timer.h"
#include "Clock.h"

void KtTimeManager::Update()
{
    const float now{ KtClock::Now() };
    delta_ = now - now_;
    now_ = now;

    updateAverageTime_.Add(delta_);

    for (auto it{ timers_.begin() }; it != timers_.end();)
    {
        const auto current{ it++ };
        current->second.Update(delta_);
    }
}

KtTimer& KtTimeManager::GetTimer(const std::string_view name)
{
    return timers_[name];
}

float KtTimeManager::Now() const
{
    return now_;
}

float KtTimeManager::Delta() const
{
    return delta_;
}

float KtTimeManager::AverageUpdateTime() const
{
    return updateAverageTime_.Get();
}
