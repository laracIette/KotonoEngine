#include "Keyboard.h"
#include "Framework.h"
#include "Window.h"
#include <GLFW/glfw3.h>

void KtKeyboard::Update()
{
    for (auto& [key, events] : _events)
    {
        std::unordered_set<KtInputState> keyStates;

        int glfwKeyState = glfwGetKey(Framework.GetWindow().GetGLFWWindow(), key);
        switch (glfwKeyState)
        {
        case GLFW_PRESS:
            keyStates.insert(KT_INPUT_STATE_DOWN);
            if (!_keyStates[key].contains(KT_INPUT_STATE_DOWN))
            {
                keyStates.insert(KT_INPUT_STATE_PRESSED);
            }
            break;

        case GLFW_RELEASE:
            keyStates.insert(KT_INPUT_STATE_UP);
            if (!_keyStates[key].contains(KT_INPUT_STATE_UP))
            {
                keyStates.insert(KT_INPUT_STATE_RELEASED);
            }
            break;

        case GLFW_REPEAT:
            keyStates.insert(KT_INPUT_STATE_DOWN);
            keyStates.insert(KT_INPUT_STATE_REPEATED);
            break;

        default:
            break;
        }

        _keyStates[key] = keyStates;

        for (auto inputState : _keyStates[key])
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
    return _events[key][inputState];
}
