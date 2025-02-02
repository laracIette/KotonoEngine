#include "InputManager.h"

void KtInputManager::Init()
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
