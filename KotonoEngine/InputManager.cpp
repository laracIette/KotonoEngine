#include "InputManager.h"

void InputManager::Init()
{
}

Mouse& InputManager::GetMouse()
{
    return _mouse;
}

Keyboard& InputManager::GetKeyboard()
{
    return _keyboard;
}
