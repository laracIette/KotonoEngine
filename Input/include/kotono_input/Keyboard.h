#pragma once
#include "InputState.h"
#include "Key.h"
#include <array>
#include <kotono_common/Event.h>
#include <kotono_common/types.h>
struct GLFWwindow;
class UWindow;
class UKeyboard final
{
public:	
	using EventKeyType = UEvent<EKey, EInputState>;

public:
	UKeyboard(UWindow& window);

	void Init();
	void Cleanup();

	void Update();

	auto GetKeyState(EKey key, EInputState inputState) const -> b8;

	auto GetEventKey() -> EventKeyType& { return eventKey_; }

private:
	void UpdateKey(GLFWwindow* window, EKey key, i32 action);

private:
	UWindow& window_;

	EventKeyType eventKey_;

	std::array<std::array<b8, InputStateCount>, KeyCount> keyStates_;
};
