#include "Keyboard.h"
#include "Framework.h"
#include "Window.h"
#include <GLFW/glfw3.h>

void KtKeyboard::Update()
{
    for (auto& [key, events] : events_)
    {
        std::unordered_set<KtInputState> keyStates;

        int glfwKeyState = glfwGetKey(Framework.GetWindow().GetGLFWWindow(), key);
        switch (glfwKeyState)
        {
        case GLFW_PRESS:
        {
            keyStates.insert(KT_INPUT_STATE_DOWN);
            if (!keyStates_[key].contains(KT_INPUT_STATE_DOWN))
            {
                keyStates.insert(KT_INPUT_STATE_PRESSED);
            }
            break;
        }
        case GLFW_RELEASE:
        {
            keyStates.insert(KT_INPUT_STATE_UP);
            if (!keyStates_[key].contains(KT_INPUT_STATE_UP))
            {
                keyStates.insert(KT_INPUT_STATE_RELEASED);
            }
            break;
        }
        default:
            break;
        }

        keyStates_[key] = keyStates;

        for (auto inputState : keyStates_[key])
        {
            if (events.contains(inputState)) 
            {
                events[inputState].Broadcast();
            }
        }
    }
}

KtEvent<>& KtKeyboard::GetEvent(const KtKey key, const KtInputState inputState)
{
    return events_[key][inputState];
}
