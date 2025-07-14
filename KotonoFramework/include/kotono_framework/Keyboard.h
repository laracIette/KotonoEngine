#pragma once
#include <map>
#include <unordered_set>
#include <array>
#include "Event.h"
#include "Key.h"
#include "InputState.h"
class KtKeyboard final
{
public:
	void Init();
	void Update();
	void UpdateKey(const KtKey key, const int action);

	KtEvent<>& GetEvent(const KtKey key, const KtInputState inputState);

private:
	std::array<std::array<KtEvent<>, KT_INPUT_STATE_COUNT>, KT_KEY_COUNT> keyEvents_;
	std::array<std::unordered_set<KtInputState>, KT_KEY_COUNT> keyStates_;
};

