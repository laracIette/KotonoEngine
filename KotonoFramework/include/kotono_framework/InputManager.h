#pragma once
#include "Mouse.h"
#include "Keyboard.h"
class KtInputManager final
{
public:
	void Init();
	void Update();
	void Cleanup();

	KtMouse& GetMouse();
	KtKeyboard& GetKeyboard();

private:
	KtMouse mouse_;
	KtKeyboard keyboard_;
};

