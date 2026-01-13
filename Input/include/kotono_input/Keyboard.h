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

	UEvent<EKey, EInputState>& EventKey();

private:
	void UpdateKey(const EKey key, const int action);

private:
	std::array<std::array<UEvent<>, InputStateCount>, KeyCount> keyEvents_;
	std::array<std::array<bool, InputStateCount>, KeyCount> keyStates_;

	UEvent<EKey, EInputState> eventKey_;
};

inline SKeyboard Keyboard;
