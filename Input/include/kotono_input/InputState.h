#pragma once
#include <kotono_common/types.h>
enum class EInputState : u8
{
	// Occurs the first frame where the input is down.
	Pressed,
	// Occurs the first frame where the input is up.
	Released,
	// Occurs every frame where the input is down.
	Down
};

inline constexpr size InputStateCount{ 3 };

inline constexpr size to_index(const EInputState inputState)
{
	return static_cast<size>(inputState);
}
