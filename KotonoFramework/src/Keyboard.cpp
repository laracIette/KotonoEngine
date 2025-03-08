#include "Keyboard.h"
#include "Framework.h"

void KtKeyboard::Update()
{
    for (auto& [key, event] : _events)
    {
        if (glfwGetKey(Framework.GetWindow().GetGLFWWindow(), key) == GLFW_PRESS)
        {
            event.Broadcast();
        }
    }
}

KtEvent& KtKeyboard::GetEvent(const KtKey key)
{
    return _events[key];
}
