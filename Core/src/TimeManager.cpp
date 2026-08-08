#include "TimeManager.h"
#include <kotono_graphics/RenderContext.h>
#include <kotono_timing/TimerManager.h>
#include <kotono_timing/TimeContext.h>
#include <kotono_timing/Stopwatch.h>
#include <kotono_timing/Clock.h>
#include "Game.h"

void GTimeManager::Init()
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

void GTimeManager::Update()
{
	const f32 now{ SClock::Now() };
	delta_ = now - now_;
	now_ = now;

	averageUpdateTime_.Add(delta_);

	TimerManager.Update(delta_);

	if (gameTime_.Update(delta_))
	{
		const f32 gameTime{ UStopwatch::Time([delta = gameTime_.lastDelta]() {
			Game.Update(delta); 
		}) };
		averageGameTime_.Add(gameTime);
	}

	if (renderTime_.Update(delta_))
	{
		const f32 renderTime{ UStopwatch::Time([]() {
			RenderContext->DrawFrame();
		}) };
		averageRenderTime_.Add(renderTime);
	}
}

f32 GTimeManager::Now() const
{
	return now_;
}

f32 GTimeManager::Delta() const
{
	return delta_;
}

UTimeContext& GTimeManager::GameTime()
{
	return gameTime_;
}

UTimeContext& GTimeManager::RenderTime()
{
	return renderTime_;
}

f32 GTimeManager::AverageUpdateTime() const
{
	return averageUpdateTime_.Get();
}

f32 GTimeManager::AverageGameTime() const
{
	return averageGameTime_.Get();
}

f32 GTimeManager::AverageRenderTime() const
{
	return averageRenderTime_.Get();
}