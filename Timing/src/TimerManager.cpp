#include "TimerManager.h"
#include "Timer.h"

void KtTimerManager::Update(const float delta)
{
    for (auto it{ timers_.begin() }; it != timers_.end();)
    {
        const auto current{ it++ };
        current->second.Update(delta);
    }
}

KtTimer& KtTimerManager::GetTimer(const std::string_view name)
{
    return timers_[name];
}
