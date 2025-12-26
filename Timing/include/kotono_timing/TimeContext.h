#pragma once
#include <cstdint>
enum class ETimeContextState : uint8_t
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