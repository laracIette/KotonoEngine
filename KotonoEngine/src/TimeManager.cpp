#include "TimeManager.h"
#include <kotono_framework/Framework.h>
#include <kotono_framework/InputManager.h>
#include <kotono_framework/Clock.h>
#include <kotono_framework/Stopwatch.h>
#include <kotono_framework/Renderer.h>
#include "Engine.h"
#include "ObjectManager.h"
#include "Game.h"
#include "log.h"

void STimeManager::Init()
{
	currentTime_ = Framework.Clock().Now();

	editorTime_ = {
		.frequency = 1.0f / 10000.0f,
		.lastDelta = 0.0f,
		.currentDelta = 0.0f,
		.scale = 1.0f,
		.total = 0.0f,
		.state = TimeContextState::Playing,
	};
	gameTime_ = {
		.frequency = 1.0f / 60.0f,
		.lastDelta = 0.0f,
		.currentDelta = 0.0f,
		.scale = 1.0f,
		.total = 0.0f,
		.state = TimeContextState::Paused,
	};
	renderTime_ = {
		.frequency = 1.0f / 120.0f,
		.lastDelta = 0.0f,
		.currentDelta = 0.0f,
		.scale = 1.0f,
		.total = 0.0f,
		.state = TimeContextState::Playing,
	};
}

void STimeManager::Update()
{
	const float now{ Framework.Clock().Now() };
	const float delta{ now - currentTime_ };
	currentTime_ = now;

	engineAverageTime_.Add(delta);

	Framework.InputManager().Mouse().Update();
	Framework.InputManager().Keyboard().Update();

	if (UpdateTimeContext(editorTime_, delta))
	{
		const float editorTime{ KtStopwatch::Time(KtDelegate(&Engine.ObjectManager(), &SObjectManager::Update)) };
		editorAverageTime_.Add(editorTime);
	}
	if (UpdateTimeContext(gameTime_, delta))
	{
		const float gameTime{ KtStopwatch::Time(KtDelegate(&Engine.Game(), &SGame::Update)) };
		gameAverageTime_.Add(gameTime);
	}
	if (UpdateTimeContext(renderTime_, delta))
	{
		const float renderTime{ KtStopwatch::Time(KtDelegate(&Framework.Renderer(), &KtRenderer::DrawFrame)) };
		renderAverageTime_.Add(renderTime);
	}
}

float STimeManager::EditorDelta() const
{
	return editorTime_.lastDelta;
}

float STimeManager::GameDelta() const
{
	return gameTime_.lastDelta;
}

float STimeManager::RenderDelta() const
{
	return renderTime_.lastDelta;
}

float STimeManager::EditorNow() const
{
	return editorTime_.total;
}

float STimeManager::GameNow() const
{
	return gameTime_.total;
}

float STimeManager::RenderNow() const
{
	return renderTime_.total;
}

float STimeManager::AverageEngineTime() const
{
	return engineAverageTime_.Get();
}

float STimeManager::AverageEditorTime() const
{
	return editorAverageTime_.Get();
}

float STimeManager::AverageGameTime() const
{
	return gameAverageTime_.Get();
}

float STimeManager::AverageRenderTime() const
{
	return renderAverageTime_.Get();
}

bool STimeManager::UpdateTimeContext(TimeContext& timeContext, const float delta) const
{
	if (timeContext.state == TimeContextState::Paused)
	{
		return false;
	}

	timeContext.total += delta * timeContext.scale;
	timeContext.currentDelta += delta * timeContext.scale;

	if (timeContext.currentDelta >= timeContext.frequency)
	{
		timeContext.lastDelta = timeContext.currentDelta;
		timeContext.currentDelta = 0;
		return true;
	}

	return false;
}
