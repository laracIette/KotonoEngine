#include "TimeManager.h"
#include <kotono_graphics/Camera.h>
#include <kotono_graphics/Renderer.h>
#include <kotono_platform/WindowViewport.h>
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
	const float now{ SClock::Now() };
	delta_ = now - now_;
	now_ = now;

	averageUpdateTime_.Add(delta_);

	TimerManager.Update(delta_);

	if (gameTime_.Update(delta_))
	{
		const float gameTime{ UStopwatch::Time([delta = gameTime_.lastDelta]() {
			Game.Update(delta); 
		}) };
		averageGameTime_.Add(gameTime);
	}

	if (renderTime_.Update(delta_))
	{
		const float renderTime{ UStopwatch::Time([]() { 
			Renderer.DrawFrame({
				.view = SCamera::GetViewMatrix(),
				.proj = SCamera::GetProjectionMatrix(),
				.viewPos = SCamera::GetPosition(),
				.windowSize = SWindowViewport::GetExtent(),
				.time = SClock::Now(),
			});
		}) };
		averageRenderTime_.Add(renderTime);
	}
}

float GTimeManager::Now() const
{
	return now_;
}

float GTimeManager::Delta() const
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

float GTimeManager::AverageUpdateTime() const
{
	return averageUpdateTime_.Get();
}

float GTimeManager::AverageGameTime() const
{
	return averageGameTime_.Get();
}

float GTimeManager::AverageRenderTime() const
{
	return averageRenderTime_.Get();
}