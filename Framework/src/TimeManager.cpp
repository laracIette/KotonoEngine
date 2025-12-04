#include "TimeManager.h"
#include "Timer.h"
#include "Framework.h"
#include "Renderer.h"
#include "Clock.h"
#include "Stopwatch.h"

void KtTimeManager::Init()
{
    renderTime_ = {
        .frequency = 1.0f / 120.0f,
        .lastDelta = 0.0f,
        .currentDelta = 0.0f,
        .scale = 1.0f,
        .total = 0.0f,
        .state = KT_TIME_CONTEXT_STATE_PLAYING,
    };
}

void KtTimeManager::Update()
{
    const float now{ Framework.Clock().Now() };
    delta_ = now - now_;
    now_ = now;

    updateAverageTime_.Add(delta_);

    for (auto it{ timers_.begin() }; it != timers_.end();)
    {
        const auto current{ it++ };
        current->second.Update();
    }

    if (renderTime_.Update(delta_))
    {
        const float renderTime{ KtStopwatch::Time(KtDelegate(&Framework.Renderer(), &KtRenderer::DrawFrame)) };
        averageRenderTime_.Add(renderTime);
    }
}

KtTimer& KtTimeManager::GetTimer(std::string_view name)
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

KtTimeContext& KtTimeManager::RenderTime()
{
    return renderTime_;
}

float KtTimeManager::AverageUpdateTime() const
{
    return updateAverageTime_.Get();
}

float KtTimeManager::AverageRenderTime() const
{
    return averageRenderTime_.Get();
}
