#pragma once
#include <array>
#include <kotono_framework/Event.h>
#include <kotono_framework/InputState.h>
#include <kotono_framework/Key.h>
class SInputManager final
{
	friend class SEngine;

public:
	KtEvent<>& EventKey(const KtKey key, const KtInputState inputState);

private:
	std::array<std::array<KtEvent<>, KT_INPUT_STATE_COUNT>, KT_KEY_COUNT> keyEvents_;
};