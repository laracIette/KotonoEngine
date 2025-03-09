#include "InputManager.h"

void KtInputManager::Init()
{
}

void KtInputManager::Cleanup()
{
}

KtMouse& KtInputManager::GetMouse()
{
    return _mouse;
}

KtKeyboard& KtInputManager::GetKeyboard()
{
    return _keyboard;
}
