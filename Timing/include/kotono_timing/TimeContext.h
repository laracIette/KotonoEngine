#pragma once
#include <kotono_common/types.h>
enum class ETimeContextState : u8
{
	Playing,
	Paused
};

struct UTimeContext final
{
	f32 frequency;
	f32 lastDelta;
	f32 currentDelta;
	f32 scale;
	f32 total;
	ETimeContextState state;

	/// <summary>
	/// Returns true if the time context updated, else false.
	/// </summary>
	bool Update(f32 deltaTime);
};