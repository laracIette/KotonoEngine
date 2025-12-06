#include "TimerManager.h"
#include "Timer.h"
#include "Clock.h"

void KtTimerManager::Update()
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

KtTimer& KtTimerManager::GetTimer(const std::string_view name)
{
    return timers_[name];
}

float KtTimerManager::Now() const
{
    return now_;
}

float KtTimerManager::Delta() const
{
    return delta_;
}

float KtTimerManager::AverageUpdateTime() const
{
    return updateAverageTime_.Get();
}
