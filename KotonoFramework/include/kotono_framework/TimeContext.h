#pragma once
enum KtTimeContextState : char
{
	KT_TIME_CONTEXT_STATE_PLAYING,
	KT_TIME_CONTEXT_STATE_PAUSED,
};

struct KtTimeContext final
{
	float frequency;
	float lastDelta;
	float currentDelta;
	float scale;
	float total;
	KtTimeContextState state;

	/// <summary>
	/// Returns true if the time context updated, else false.
	/// </summary>
	bool Update(const float delta);
};