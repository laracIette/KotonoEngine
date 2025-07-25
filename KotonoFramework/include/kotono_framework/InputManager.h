#pragma once
#include "Mouse.h"
#include "Keyboard.h"
class KtInputManager final
{
	friend class KtFramework;

private:
	void Init();
	void Update();

public:
	KtMouse& GetMouse();
	KtKeyboard& GetKeyboard();

private:
	KtMouse mouse_;
	KtKeyboard keyboard_;
};

