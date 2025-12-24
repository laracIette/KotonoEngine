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

	KtEvent<>& EventKey(const EKey key, const EInputState inputState);
	bool KeyState(const EKey key, const EInputState inputState) const;

private:
	std::array<std::array<KtEvent<>, InputStateCount>, KeyCount> keyEvents_;
	std::array<std::array<bool, InputStateCount>, KeyCount> keyStates_;

	void UpdateKey(const EKey key, const int action);
};

inline SKeyboard Keyboard;
