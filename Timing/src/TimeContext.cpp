#include "TimeContext.h"

bool KtTimeContext::Update(const float delta)
{
	if (state == ETimeContextState::Paused)
	{
		return false;
	}

	total += delta * scale;
	currentDelta += delta * scale;

	if (currentDelta >= frequency)
	{
		lastDelta = currentDelta;
		currentDelta = 0;
		return true;
	}

	return false;
}