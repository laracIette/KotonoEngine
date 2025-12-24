#pragma once
#include <cstdint>
enum class EInputState : uint8_t
{
	// Occurs the first frame where the input is down.
	Pressed,
	// Occurs the first frame where the input is up.
	Released,
	// Occurs every frame where the input is down.
	Down
};

inline constexpr size_t InputStateCount{ 3 };

constexpr size_t to_index(const EInputState inputState)
{
	return static_cast<size_t>(inputState);
}
