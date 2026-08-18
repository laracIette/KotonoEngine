#include "TimeContext.h"

bool UTimeContext::Update(f32 deltaTime)
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
		currentDelta = 0.0f;
		return true;
	}

	return false;
}