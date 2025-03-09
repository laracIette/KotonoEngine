#pragma once
#include "Mouse.h"
#include "Keyboard.h"
class KtInputManager final
{
public:
	void Init();
	void Cleanup();

	KtMouse& GetMouse();
	KtKeyboard& GetKeyboard();

private:
	KtMouse _mouse;
	KtKeyboard _keyboard;
};

