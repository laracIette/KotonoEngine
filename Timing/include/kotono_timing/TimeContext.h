#pragma once
#include <kotono_common/types.h>
enum class ETimeContextState : u8
{
	Playing,
	Paused
};

struct KtTimeContext final
{
	float frequency;
	float lastDelta;
	float currentDelta;
	float scale;
	float total;
	ETimeContextState state;

	/// <summary>
	/// Returns true if the time context updated, else false.
	/// </summary>
	bool Update(const float deltaTime);
};