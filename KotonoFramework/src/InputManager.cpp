#include "InputManager.h"

void KtInputManager::Init()
{
}

void KtInputManager::Update()
{
    mouse_.Update();
    keyboard_.Update();
}

void KtInputManager::Cleanup()
{
}

KtMouse& KtInputManager::GetMouse()
{
    return mouse_;
}

KtKeyboard& KtInputManager::GetKeyboard()
{
    return keyboard_;
}
