#include "InputManager.h"

void KtInputManager::Init()
{
    keyboard_.Init();
    mouse_.Init();
}

void KtInputManager::Update()
{
    keyboard_.Update();
    mouse_.Update();
}

KtMouse& KtInputManager::Mouse()
{
    return mouse_;
}

const KtMouse& KtInputManager::Mouse() const
{
    return mouse_;
}

KtKeyboard& KtInputManager::Keyboard()
{
    return keyboard_;
}

const KtKeyboard& KtInputManager::Keyboard() const
{
    return keyboard_;
}
