#include "TimerManager.h"

UTimer& GTimerManager::GetTimer(const std::string_view name)
{
    return timers_[name];
}
void GTimerManager::Update(const f32 deltaTime)
{
    for (auto it{ timers_.begin() }; it != timers_.end();)
    {
        const auto current{ it++ };
        current->second.Update(deltaTime);
    }
}
