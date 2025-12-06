#pragma once
#include "Mouse.h"
#include "Keyboard.h"
class KtInputManager final
{
	friend class SCore;

private:
	void Init();
	void Update();

public:
	KtMouse& Mouse();
	const KtMouse& Mouse() const;
	KtKeyboard& Keyboard();
	const KtKeyboard& Keyboard() const;

private:
	KtMouse mouse_;
	KtKeyboard keyboard_;
};

inline KtInputManager InputManager;
