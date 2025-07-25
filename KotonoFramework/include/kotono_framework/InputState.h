#pragma once
enum KtInputState : char
{
	// Occurs the first frame where the input is down.
	KT_INPUT_STATE_PRESSED,
	// Occurs the first frame where the input is up.
	KT_INPUT_STATE_RELEASED,
	// Occurs every frame where the input is down.
	KT_INPUT_STATE_DOWN,

	KT_INPUT_STATE_COUNT = 3
};