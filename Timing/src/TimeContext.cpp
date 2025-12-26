#include "TimeContext.h"

bool KtTimeContext::Update(const float deltaTime)
{
	if (state == ETimeContextState::Paused)
	{
		return false;
	}

	total += deltaTime * scale;
	currentDelta += deltaTime * scale;

	if (currentDelta >= frequency)
	{
		lastDelta = currentDelta;
		currentDelta = 0;
		return true;
	}

	return false;
}