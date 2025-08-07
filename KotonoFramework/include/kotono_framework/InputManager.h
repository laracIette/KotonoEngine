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
	const KtMouse& GetMouse() const;
	KtKeyboard& GetKeyboard();
	const KtKeyboard& GetKeyboard() const;

private:
	KtMouse mouse_;
	KtKeyboard keyboard_;
};

