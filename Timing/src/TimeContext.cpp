#include "TimeContext.h"

bool KtTimeContext::Update(const float delta)
{
	if (state == KT_TIME_CONTEXT_STATE_PAUSED)
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