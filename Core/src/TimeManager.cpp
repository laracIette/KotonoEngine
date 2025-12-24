#include "TimeManager.h"
#include <kotono_graphics/Renderer.h>
#include <kotono_common/Delegate.h>
#include <kotono_timing/TimerManager.h>
#include <kotono_timing/TimeContext.h>
#include <kotono_timing/Stopwatch.h>
#include <kotono_timing/Clock.h>
#include "Game.h"

void STimeManager::Init()
{
	gameTime_ = {
		.frequency = 1.0f / 120.0f,
		.lastDelta = 0.0f,
		.currentDelta = 0.0f,
		.scale = 1.0f,
		.total = 0.0f,
		.state = ETimeContextState::Paused,
	};

	renderTime_ = {
		.frequency = 1.0f / 60.0f,
		.lastDelta = 0.0f,
		.currentDelta = 0.0f,
		.scale = 1.0f,
		.total = 0.0f,
		.state = ETimeContextState::Playing,
	};
}

void STimeManager::Update()
{
	const float now{ KtClock::Now() };
	delta_ = now - now_;
	now_ = now;

	averageUpdateTime_.Add(delta_);

	TimerManager.Update(delta_);

	if (gameTime_.Update(delta_))
	{
		const float gameTime{ KtStopwatch::Time(KtDelegate(&Game, &SGame::Update)) };
		averageGameTime_.Add(gameTime);
	}

	if (renderTime_.Update(delta_))
	{
		const float renderTime{ KtStopwatch::Time(KtDelegate(&Renderer, &KtRenderer::DrawFrame)) };
		averageRenderTime_.Add(renderTime);
	}
}

float STimeManager::Now() const
{
	return now_;
}

float STimeManager::Delta() const
{
	return delta_;
}

KtTimeContext& STimeManager::GameTime()
{
	return gameTime_;
}

KtTimeContext& STimeManager::RenderTime()
{
	return renderTime_;
}

float STimeManager::AverageUpdateTime() const
{
	return averageUpdateTime_.Get();
}

float STimeManager::AverageGameTime() const
{
	return averageGameTime_.Get();
}

float STimeManager::AverageRenderTime() const
{
	return averageRenderTime_.Get();
}