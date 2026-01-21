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

	UEvent<>& EventKey(const EKey key, const EInputState inputState);
	bool KeyState(const EKey key, const EInputState inputState) const;

	UEvent<EKey>& EventAnyKey(const EInputState inputState);

private:
	void UpdateKey(const EKey key, const i32 action);

private:
	std::array<std::array<UEvent<>, InputStateCount>, KeyCount> keyEvents_;
	std::array<std::array<bool, InputStateCount>, KeyCount> keyStates_;

	std::array<UEvent<EKey>, InputStateCount> anyKeyEvents_;
};

inline SKeyboard Keyboard;
