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
