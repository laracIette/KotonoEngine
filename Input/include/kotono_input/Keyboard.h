#pragma once
#include "InputState.h"
#include "Key.h"
#include <array>
#include <kotono_common/Event.h>
#include <kotono_common/types.h>
struct GLFWwindow;
class UWindow;
class GKeyboard final
{
	friend void key_callback_(GLFWwindow* window, i32 key, i32 scancode, i32 action, i32 mods);

public:
	void Init(UWindow& window);
	void Update();

	UEvent<>& GetEventKey(EKey key, EInputState inputState);
	b8 GetKeyState(EKey key, EInputState inputState) const;

	UEvent<EKey>& GetEventAnyKey(EInputState inputState);

private:
	void UpdateKey(EKey key, i32 action);

private:
	std::array<std::array<UEvent<>, InputStateCount>, KeyCount> keyEvents_;
	std::array<std::array<b8, InputStateCount>, KeyCount> keyStates_;

	std::array<UEvent<EKey>, InputStateCount> anyKeyEvents_;
};

inline GKeyboard Keyboard;
