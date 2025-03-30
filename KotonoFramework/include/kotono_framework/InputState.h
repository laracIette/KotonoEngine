#pragma once
enum KtInputState
{
	// Occurs the first frame where the input is down.
	KT_INPUT_STATE_PRESSED,
	// Occurs the first frame where the input is up.
	KT_INPUT_STATE_RELEASED,
	// Occurs every frame where the input is down.
	KT_INPUT_STATE_DOWN,
	// Occurs every frame where the input is up.
	KT_INPUT_STATE_UP,
	// Occurs from the second frame where the input is down.
	KT_INPUT_STATE_REPEATED
};