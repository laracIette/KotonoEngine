#include "TimeManager.h"
#include <kotono_framework/Framework.h>
#include <kotono_framework/TimeManager.h>
#include <kotono_framework/Stopwatch.h>
#include "Engine.h"
#include "Game.h"
#include "log.h"

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
}

void STimeManager::Update()
{
	const float delta{ Framework.TimeManager().Delta() };

	if (gameTime_.Update(delta))
	{
		const float gameTime{ KtStopwatch::Time(KtDelegate(&Engine.Game(), &SGame::Update)) };
		averageGameTime_.Add(gameTime);
	}
}

KtTimeContext& STimeManager::GameTime()
{
	return gameTime_;
}

float STimeManager::AverageGameTime() const
{
	return averageGameTime_.Get();
}
