#include "TimeManager.h"
#include <kotono_graphics/Renderer.h>
#include <kotono_timing/TimeManager.h>
#include <kotono_timing/TimeContext.h>
#include <kotono_timing/Stopwatch.h>
#include "Engine.h"
#include "Game.h"

void STimeManager::Init()
{
	gameTime_ = {
		.frequency = 1.0f / 120.0f,
		.lastDelta = 0.0f,
		.currentDelta = 0.0f,
		.scale = 1.0f,
		.total = 0.0f,
		.state = KT_TIME_CONTEXT_STATE_PAUSED,
	};

	renderTime_ = {
		.frequency = 1.0f / 60.0f,
		.lastDelta = 0.0f,
		.currentDelta = 0.0f,
		.scale = 1.0f,
		.total = 0.0f,
		.state = KT_TIME_CONTEXT_STATE_PLAYING,
	};
}

void STimeManager::Update()
{
	const float delta{ TimeManager.Delta() };

	if (gameTime_.Update(delta))
	{
		const float gameTime{ KtStopwatch::Time(KtDelegate(&Engine.Game(), &SGame::Update)) };
		averageGameTime_.Add(gameTime);
	}

	if (renderTime_.Update(delta))
	{
		const float renderTime{ KtStopwatch::Time(KtDelegate(&Renderer, &KtRenderer::DrawFrame)) };
		averageRenderTime_.Add(renderTime);
	}
}

KtTimeContext& STimeManager::GameTime()
{
	return gameTime_;
}

KtTimeContext& STimeManager::RenderTime()
{
	return renderTime_;
}

float STimeManager::AverageGameTime() const
{
	return averageGameTime_.Get();
}

float STimeManager::AverageRenderTime() const
{
	return averageRenderTime_.Get();
}
