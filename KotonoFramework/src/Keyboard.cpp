#include "Keyboard.h"
#include "Framework.h"
#include "Window.h"
#include "InputManager.h"
#include <GLFW/glfw3.h>

void key_callback_(GLFWwindow* window, int key, int scancode, int action, int mods);
const int keyToGLFWKey(const KtKey key);
const KtKey GLFWKeyToKey(const int key);

void KtKeyboard::Init()
{
    glfwSetKeyCallback(Framework.GetWindow().GetGLFWWindow(), key_callback_);
}

void KtKeyboard::Update()
{
    for (size_t key = 0; key < keyStates_.size(); key++)
    {
        if (keyStates_[key].empty())
        {
            continue;
        }

        for (const auto inputState : keyStates_[key])
        {
            keyEvents_[key][inputState].Broadcast();
        }

        std::erase_if(keyStates_[key],
            [](const KtInputState inputState)
            {
                return inputState == KT_INPUT_STATE_PRESSED || inputState == KT_INPUT_STATE_RELEASED;
            }
        );
    }
}

void KtKeyboard::UpdateKey(const KtKey key, const int action)
{
    std::unordered_set<KtInputState> keyStates;
    
    switch (action)
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
        keyStates.insert(KT_INPUT_STATE_RELEASED);
        break;
    }
    default:
        break;
    }

    keyStates_[key] = keyStates;
}

KtEvent<>& KtKeyboard::GetEvent(const KtKey key, const KtInputState inputState)
{
    return keyEvents_[key][inputState];
}

void key_callback_(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_REPEAT)
    {
        return;
    }

    Framework.GetInputManager().GetKeyboard().UpdateKey(GLFWKeyToKey(key), action);
}

const int keyToGLFWKey(const KtKey key)
{
    switch (key)
    {
    case KT_KEY_SPACE: return GLFW_KEY_SPACE;
    case KT_KEY_APOSTROPHE: return GLFW_KEY_APOSTROPHE;
    case KT_KEY_COMMA: return GLFW_KEY_COMMA;
    case KT_KEY_MINUS: return GLFW_KEY_MINUS;
    case KT_KEY_PERIOD: return GLFW_KEY_PERIOD;
    case KT_KEY_SLASH: return GLFW_KEY_SLASH;
    case KT_KEY_0: return GLFW_KEY_0;
    case KT_KEY_1: return GLFW_KEY_1;
    case KT_KEY_2: return GLFW_KEY_2;
    case KT_KEY_3: return GLFW_KEY_3;
    case KT_KEY_4: return GLFW_KEY_4;
    case KT_KEY_5: return GLFW_KEY_5;
    case KT_KEY_6: return GLFW_KEY_6;
    case KT_KEY_7: return GLFW_KEY_7;
    case KT_KEY_8: return GLFW_KEY_8;
    case KT_KEY_9: return GLFW_KEY_9;
    case KT_KEY_SEMICOLON: return GLFW_KEY_SEMICOLON;
    case KT_KEY_EQUAL: return GLFW_KEY_EQUAL;
    case KT_KEY_A: return GLFW_KEY_A;
    case KT_KEY_B: return GLFW_KEY_B;
    case KT_KEY_C: return GLFW_KEY_C;
    case KT_KEY_D: return GLFW_KEY_D;
    case KT_KEY_E: return GLFW_KEY_E;
    case KT_KEY_F: return GLFW_KEY_F;
    case KT_KEY_G: return GLFW_KEY_G;
    case KT_KEY_H: return GLFW_KEY_H;
    case KT_KEY_I: return GLFW_KEY_I;
    case KT_KEY_J: return GLFW_KEY_J;
    case KT_KEY_K: return GLFW_KEY_K;
    case KT_KEY_L: return GLFW_KEY_L;
    case KT_KEY_M: return GLFW_KEY_M;
    case KT_KEY_N: return GLFW_KEY_N;
    case KT_KEY_O: return GLFW_KEY_O;
    case KT_KEY_P: return GLFW_KEY_P;
    case KT_KEY_Q: return GLFW_KEY_Q;
    case KT_KEY_R: return GLFW_KEY_R;
    case KT_KEY_S: return GLFW_KEY_S;
    case KT_KEY_T: return GLFW_KEY_T;
    case KT_KEY_U: return GLFW_KEY_U;
    case KT_KEY_V: return GLFW_KEY_V;
    case KT_KEY_W: return GLFW_KEY_W;
    case KT_KEY_X: return GLFW_KEY_X;
    case KT_KEY_Y: return GLFW_KEY_Y;
    case KT_KEY_Z: return GLFW_KEY_Z;
    case KT_KEY_LEFT_BRACKET: return GLFW_KEY_LEFT_BRACKET;
    case KT_KEY_BACKSLASH: return GLFW_KEY_BACKSLASH;
    case KT_KEY_RIGHT_BRACKET: return GLFW_KEY_RIGHT_BRACKET;
    case KT_KEY_GRAVE_ACCENT: return GLFW_KEY_GRAVE_ACCENT;
    case KT_KEY_WORLD_1: return GLFW_KEY_WORLD_1;
    case KT_KEY_WORLD_2: return GLFW_KEY_WORLD_2;
    case KT_KEY_ESCAPE: return GLFW_KEY_ESCAPE;
    case KT_KEY_ENTER: return GLFW_KEY_ENTER;
    case KT_KEY_TAB: return GLFW_KEY_TAB;
    case KT_KEY_BACKSPACE: return GLFW_KEY_BACKSPACE;
    case KT_KEY_INSERT: return GLFW_KEY_INSERT;
    case KT_KEY_DELETE: return GLFW_KEY_DELETE;
    case KT_KEY_RIGHT: return GLFW_KEY_RIGHT;
    case KT_KEY_LEFT: return GLFW_KEY_LEFT;
    case KT_KEY_DOWN: return GLFW_KEY_DOWN;
    case KT_KEY_UP: return GLFW_KEY_UP;
    case KT_KEY_PAGE_UP: return GLFW_KEY_PAGE_UP;
    case KT_KEY_PAGE_DOWN: return GLFW_KEY_PAGE_DOWN;
    case KT_KEY_HOME: return GLFW_KEY_HOME;
    case KT_KEY_END: return GLFW_KEY_END;
    case KT_KEY_CAPS_LOCK: return GLFW_KEY_CAPS_LOCK;
    case KT_KEY_SCROLL_LOCK: return GLFW_KEY_SCROLL_LOCK;
    case KT_KEY_NUM_LOCK: return GLFW_KEY_NUM_LOCK;
    case KT_KEY_PRINT_SCREEN: return GLFW_KEY_PRINT_SCREEN;
    case KT_KEY_PAUSE: return GLFW_KEY_PAUSE;
    case KT_KEY_F1: return GLFW_KEY_F1;
    case KT_KEY_F2: return GLFW_KEY_F2;
    case KT_KEY_F3: return GLFW_KEY_F3;
    case KT_KEY_F4: return GLFW_KEY_F4;
    case KT_KEY_F5: return GLFW_KEY_F5;
    case KT_KEY_F6: return GLFW_KEY_F6;
    case KT_KEY_F7: return GLFW_KEY_F7;
    case KT_KEY_F8: return GLFW_KEY_F8;
    case KT_KEY_F9: return GLFW_KEY_F9;
    case KT_KEY_F10: return GLFW_KEY_F10;
    case KT_KEY_F11: return GLFW_KEY_F11;
    case KT_KEY_F12: return GLFW_KEY_F12;
    case KT_KEY_F13: return GLFW_KEY_F13;
    case KT_KEY_F14: return GLFW_KEY_F14;
    case KT_KEY_F15: return GLFW_KEY_F15;
    case KT_KEY_F16: return GLFW_KEY_F16;
    case KT_KEY_F17: return GLFW_KEY_F17;
    case KT_KEY_F18: return GLFW_KEY_F18;
    case KT_KEY_F19: return GLFW_KEY_F19;
    case KT_KEY_F20: return GLFW_KEY_F20;
    case KT_KEY_F21: return GLFW_KEY_F21;
    case KT_KEY_F22: return GLFW_KEY_F22;
    case KT_KEY_F23: return GLFW_KEY_F23;
    case KT_KEY_F24: return GLFW_KEY_F24;
    case KT_KEY_F25: return GLFW_KEY_F25;
    case KT_KEY_KP_0: return GLFW_KEY_KP_0;
    case KT_KEY_KP_1: return GLFW_KEY_KP_1;
    case KT_KEY_KP_2: return GLFW_KEY_KP_2;
    case KT_KEY_KP_3: return GLFW_KEY_KP_3;
    case KT_KEY_KP_4: return GLFW_KEY_KP_4;
    case KT_KEY_KP_5: return GLFW_KEY_KP_5;
    case KT_KEY_KP_6: return GLFW_KEY_KP_6;
    case KT_KEY_KP_7: return GLFW_KEY_KP_7;
    case KT_KEY_KP_8: return GLFW_KEY_KP_8;
    case KT_KEY_KP_9: return GLFW_KEY_KP_9;
    case KT_KEY_KP_DECIMAL: return GLFW_KEY_KP_DECIMAL;
    case KT_KEY_KP_DIVIDE: return GLFW_KEY_KP_DIVIDE;
    case KT_KEY_KP_MULTIPLY: return GLFW_KEY_KP_MULTIPLY;
    case KT_KEY_KP_SUBTRACT: return GLFW_KEY_KP_SUBTRACT;
    case KT_KEY_KP_ADD: return GLFW_KEY_KP_ADD;
    case KT_KEY_KP_ENTER: return GLFW_KEY_KP_ENTER;
    case KT_KEY_KP_EQUAL: return GLFW_KEY_KP_EQUAL;
    case KT_KEY_LEFT_SHIFT: return GLFW_KEY_LEFT_SHIFT;
    case KT_KEY_LEFT_CONTROL: return GLFW_KEY_LEFT_CONTROL;
    case KT_KEY_LEFT_ALT: return GLFW_KEY_LEFT_ALT;
    case KT_KEY_LEFT_SUPER: return GLFW_KEY_LEFT_SUPER;
    case KT_KEY_RIGHT_SHIFT: return GLFW_KEY_RIGHT_SHIFT;
    case KT_KEY_RIGHT_CONTROL: return GLFW_KEY_RIGHT_CONTROL;
    case KT_KEY_RIGHT_ALT: return GLFW_KEY_RIGHT_ALT;
    case KT_KEY_RIGHT_SUPER: return GLFW_KEY_RIGHT_SUPER;
    case KT_KEY_MENU: return GLFW_KEY_MENU;
    case KT_KEY_LAST: return GLFW_KEY_LAST;
    default: return GLFW_KEY_UNKNOWN;
    }
}

const KtKey GLFWKeyToKey(const int key)
{
    switch (key)
    {
    case GLFW_KEY_SPACE: return KT_KEY_SPACE;
    case GLFW_KEY_APOSTROPHE: return KT_KEY_APOSTROPHE;
    case GLFW_KEY_COMMA: return KT_KEY_COMMA;
    case GLFW_KEY_MINUS: return KT_KEY_MINUS;
    case GLFW_KEY_PERIOD: return KT_KEY_PERIOD;
    case GLFW_KEY_SLASH: return KT_KEY_SLASH;
    case GLFW_KEY_0: return KT_KEY_0;
    case GLFW_KEY_1: return KT_KEY_1;
    case GLFW_KEY_2: return KT_KEY_2;
    case GLFW_KEY_3: return KT_KEY_3;
    case GLFW_KEY_4: return KT_KEY_4;
    case GLFW_KEY_5: return KT_KEY_5;
    case GLFW_KEY_6: return KT_KEY_6;
    case GLFW_KEY_7: return KT_KEY_7;
    case GLFW_KEY_8: return KT_KEY_8;
    case GLFW_KEY_9: return KT_KEY_9;
    case GLFW_KEY_SEMICOLON: return KT_KEY_SEMICOLON;
    case GLFW_KEY_EQUAL: return KT_KEY_EQUAL;
    case GLFW_KEY_A: return KT_KEY_A;
    case GLFW_KEY_B: return KT_KEY_B;
    case GLFW_KEY_C: return KT_KEY_C;
    case GLFW_KEY_D: return KT_KEY_D;
    case GLFW_KEY_E: return KT_KEY_E;
    case GLFW_KEY_F: return KT_KEY_F;
    case GLFW_KEY_G: return KT_KEY_G;
    case GLFW_KEY_H: return KT_KEY_H;
    case GLFW_KEY_I: return KT_KEY_I;
    case GLFW_KEY_J: return KT_KEY_J;
    case GLFW_KEY_K: return KT_KEY_K;
    case GLFW_KEY_L: return KT_KEY_L;
    case GLFW_KEY_M: return KT_KEY_M;
    case GLFW_KEY_N: return KT_KEY_N;
    case GLFW_KEY_O: return KT_KEY_O;
    case GLFW_KEY_P: return KT_KEY_P;
    case GLFW_KEY_Q: return KT_KEY_Q;
    case GLFW_KEY_R: return KT_KEY_R;
    case GLFW_KEY_S: return KT_KEY_S;
    case GLFW_KEY_T: return KT_KEY_T;
    case GLFW_KEY_U: return KT_KEY_U;
    case GLFW_KEY_V: return KT_KEY_V;
    case GLFW_KEY_W: return KT_KEY_W;
    case GLFW_KEY_X: return KT_KEY_X;
    case GLFW_KEY_Y: return KT_KEY_Y;
    case GLFW_KEY_Z: return KT_KEY_Z;
    case GLFW_KEY_LEFT_BRACKET: return KT_KEY_LEFT_BRACKET;
    case GLFW_KEY_BACKSLASH: return KT_KEY_BACKSLASH;
    case GLFW_KEY_RIGHT_BRACKET: return KT_KEY_RIGHT_BRACKET;
    case GLFW_KEY_GRAVE_ACCENT: return KT_KEY_GRAVE_ACCENT;
    case GLFW_KEY_WORLD_1: return KT_KEY_WORLD_1;
    case GLFW_KEY_WORLD_2: return KT_KEY_WORLD_2;
    case GLFW_KEY_ESCAPE: return KT_KEY_ESCAPE;
    case GLFW_KEY_ENTER: return KT_KEY_ENTER;
    case GLFW_KEY_TAB: return KT_KEY_TAB;
    case GLFW_KEY_BACKSPACE: return KT_KEY_BACKSPACE;
    case GLFW_KEY_INSERT: return KT_KEY_INSERT;
    case GLFW_KEY_DELETE: return KT_KEY_DELETE;
    case GLFW_KEY_RIGHT: return KT_KEY_RIGHT;
    case GLFW_KEY_LEFT: return KT_KEY_LEFT;
    case GLFW_KEY_DOWN: return KT_KEY_DOWN;
    case GLFW_KEY_UP: return KT_KEY_UP;
    case GLFW_KEY_PAGE_UP: return KT_KEY_PAGE_UP;
    case GLFW_KEY_PAGE_DOWN: return KT_KEY_PAGE_DOWN;
    case GLFW_KEY_HOME: return KT_KEY_HOME;
    case GLFW_KEY_END: return KT_KEY_END;
    case GLFW_KEY_CAPS_LOCK: return KT_KEY_CAPS_LOCK;
    case GLFW_KEY_SCROLL_LOCK: return KT_KEY_SCROLL_LOCK;
    case GLFW_KEY_NUM_LOCK: return KT_KEY_NUM_LOCK;
    case GLFW_KEY_PRINT_SCREEN: return KT_KEY_PRINT_SCREEN;
    case GLFW_KEY_PAUSE: return KT_KEY_PAUSE;
    case GLFW_KEY_F1: return KT_KEY_F1;
    case GLFW_KEY_F2: return KT_KEY_F2;
    case GLFW_KEY_F3: return KT_KEY_F3;
    case GLFW_KEY_F4: return KT_KEY_F4;
    case GLFW_KEY_F5: return KT_KEY_F5;
    case GLFW_KEY_F6: return KT_KEY_F6;
    case GLFW_KEY_F7: return KT_KEY_F7;
    case GLFW_KEY_F8: return KT_KEY_F8;
    case GLFW_KEY_F9: return KT_KEY_F9;
    case GLFW_KEY_F10: return KT_KEY_F10;
    case GLFW_KEY_F11: return KT_KEY_F11;
    case GLFW_KEY_F12: return KT_KEY_F12;
    case GLFW_KEY_F13: return KT_KEY_F13;
    case GLFW_KEY_F14: return KT_KEY_F14;
    case GLFW_KEY_F15: return KT_KEY_F15;
    case GLFW_KEY_F16: return KT_KEY_F16;
    case GLFW_KEY_F17: return KT_KEY_F17;
    case GLFW_KEY_F18: return KT_KEY_F18;
    case GLFW_KEY_F19: return KT_KEY_F19;
    case GLFW_KEY_F20: return KT_KEY_F20;
    case GLFW_KEY_F21: return KT_KEY_F21;
    case GLFW_KEY_F22: return KT_KEY_F22;
    case GLFW_KEY_F23: return KT_KEY_F23;
    case GLFW_KEY_F24: return KT_KEY_F24;
    case GLFW_KEY_F25: return KT_KEY_F25;
    case GLFW_KEY_KP_0: return KT_KEY_KP_0;
    case GLFW_KEY_KP_1: return KT_KEY_KP_1;
    case GLFW_KEY_KP_2: return KT_KEY_KP_2;
    case GLFW_KEY_KP_3: return KT_KEY_KP_3;
    case GLFW_KEY_KP_4: return KT_KEY_KP_4;
    case GLFW_KEY_KP_5: return KT_KEY_KP_5;
    case GLFW_KEY_KP_6: return KT_KEY_KP_6;
    case GLFW_KEY_KP_7: return KT_KEY_KP_7;
    case GLFW_KEY_KP_8: return KT_KEY_KP_8;
    case GLFW_KEY_KP_9: return KT_KEY_KP_9;
    case GLFW_KEY_KP_DECIMAL: return KT_KEY_KP_DECIMAL;
    case GLFW_KEY_KP_DIVIDE: return KT_KEY_KP_DIVIDE;
    case GLFW_KEY_KP_MULTIPLY: return KT_KEY_KP_MULTIPLY;
    case GLFW_KEY_KP_SUBTRACT: return KT_KEY_KP_SUBTRACT;
    case GLFW_KEY_KP_ADD: return KT_KEY_KP_ADD;
    case GLFW_KEY_KP_ENTER: return KT_KEY_KP_ENTER;
    case GLFW_KEY_KP_EQUAL: return KT_KEY_KP_EQUAL;
    case GLFW_KEY_LEFT_SHIFT: return KT_KEY_LEFT_SHIFT;
    case GLFW_KEY_LEFT_CONTROL: return KT_KEY_LEFT_CONTROL;
    case GLFW_KEY_LEFT_ALT: return KT_KEY_LEFT_ALT;
    case GLFW_KEY_LEFT_SUPER: return KT_KEY_LEFT_SUPER;
    case GLFW_KEY_RIGHT_SHIFT: return KT_KEY_RIGHT_SHIFT;
    case GLFW_KEY_RIGHT_CONTROL: return KT_KEY_RIGHT_CONTROL;
    case GLFW_KEY_RIGHT_ALT: return KT_KEY_RIGHT_ALT;
    case GLFW_KEY_RIGHT_SUPER: return KT_KEY_RIGHT_SUPER;
    case GLFW_KEY_MENU: return KT_KEY_MENU;
    default: return static_cast<KtKey>(-1);
    }
}
