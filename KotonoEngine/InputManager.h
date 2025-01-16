#pragma once
#include "Mouse.h"
#include "Keyboard.h"
class InputManager final
{
public:
	Mouse& GetMouse();
	Keyboard& GetKeyboard();

private:
	Mouse _mouse;
	Keyboard _keyboard;
};

