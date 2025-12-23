#pragma once
#include <map>
#include <unordered_set>
#include <array>
#include <kotono_common/Event.h>
#include "Key.h"
#include "InputState.h"
struct GLFWwindow;
class SKeyboard final
{
	friend void key_callback_(GLFWwindow* window, int key, int scancode, int action, int mods);

public:
	void Init();
	void Update();

	KtEvent<>& EventKey(const KtKey key, const KtInputState inputState);
	bool KeyState(const KtKey key, const KtInputState inputState) const;

private:
	std::array<std::array<KtEvent<>, KT_INPUT_STATE_COUNT>, KT_KEY_COUNT> keyEvents_;
	std::array<std::array<bool, KT_INPUT_STATE_COUNT>, KT_KEY_COUNT> keyStates_;

	void UpdateKey(const KtKey key, const int action);
};

inline SKeyboard Keyboard;
