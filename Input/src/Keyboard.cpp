#include "Keyboard.h"
#include <GLFW/glfw3.h>
#include <kotono_common/log.h>
#include <kotono_platform/Window.h>

#define KT_LOG_IMPORTANCE_LEVEL_KEYBOARD ELogImportanceLevel::Low

void key_callback_(GLFWwindow* window, int key, int scancode, int action, int mods);

constexpr int keyToGLFWKey(const EKey key);
constexpr EKey GLFWKeyToKey(const int key);

void SKeyboard::Init()
{
    glfwSetKeyCallback(Window.GetGLFWWindow(), key_callback_);
}

void SKeyboard::Update()
{
    for (size key{ 0 }; key < KeyCount; ++key)
    {
        for (size inputState{ 0 }; inputState < InputStateCount; ++inputState)
        {
            if (keyStates_[key][inputState])
            {
                keyEvents_[key][inputState].Broadcast();
                eventKey_.Broadcast(static_cast<EKey>(key), static_cast<EInputState>(inputState));
            }
        }

        if (keyStates_[key][to_index(EInputState::Pressed)])
        {
            keyStates_[key][to_index(EInputState::Pressed)] = false;
        }
    }
}

void SKeyboard::UpdateKey(const EKey key, const int action)
{
    const size keyIndex{ to_index(key) };

    switch (action)
    {
    case GLFW_PRESS:
    {
		KT_LOG(KT_LOG_IMPORTANCE_LEVEL_KEYBOARD, "Input.SKeyboard::UpdateKey()", "GLFW_PRESS key %d", key);

        keyStates_[keyIndex][to_index(EInputState::Released)] = false;

        keyStates_[keyIndex][to_index(EInputState::Pressed)] = true;
        keyStates_[keyIndex][to_index(EInputState::Down)] = true;
        break;
    }
    case GLFW_RELEASE:
    {
        KT_LOG(KT_LOG_IMPORTANCE_LEVEL_KEYBOARD, "Input.SKeyboard::UpdateKey()", "GLFW_RELEASE key %d", key);

        keyStates_[keyIndex][to_index(EInputState::Pressed)] = false;
        keyStates_[keyIndex][to_index(EInputState::Down)] = false;

        keyStates_[keyIndex][to_index(EInputState::Released)] = true;
        break;
    }
    default:
        break;
    }
}

UEvent<>& SKeyboard::EventKey(const EKey key, const EInputState inputState)
{
    return keyEvents_[to_index(key)][to_index(inputState)];
}

bool SKeyboard::KeyState(const EKey key, const EInputState inputState) const
{
    return keyStates_[to_index(key)][to_index(inputState)];
}

UEvent<EKey, EInputState>& SKeyboard::EventKey()
{
    return eventKey_;
}

void key_callback_(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_REPEAT)
    {
        return;
    }

    Keyboard.UpdateKey(GLFWKeyToKey(key), action);
}

constexpr int keyToGLFWKey(const EKey key)
{
    switch (key)
    {
    case EKey::Space: return GLFW_KEY_SPACE;
    case EKey::Apostrophe: return GLFW_KEY_APOSTROPHE;
    case EKey::Comma: return GLFW_KEY_COMMA;
    case EKey::Minus: return GLFW_KEY_MINUS;
    case EKey::Period: return GLFW_KEY_PERIOD;
    case EKey::Slash: return GLFW_KEY_SLASH;
    case EKey::Num0: return GLFW_KEY_0;
    case EKey::Num1: return GLFW_KEY_1;
    case EKey::Num2: return GLFW_KEY_2;
    case EKey::Num3: return GLFW_KEY_3;
    case EKey::Num4: return GLFW_KEY_4;
    case EKey::Num5: return GLFW_KEY_5;
    case EKey::Num6: return GLFW_KEY_6;
    case EKey::Num7: return GLFW_KEY_7;
    case EKey::Num8: return GLFW_KEY_8;
    case EKey::Num9: return GLFW_KEY_9;
    case EKey::Semicolon: return GLFW_KEY_SEMICOLON;
    case EKey::Equal: return GLFW_KEY_EQUAL;
    case EKey::A: return GLFW_KEY_A;
    case EKey::B: return GLFW_KEY_B;
    case EKey::C: return GLFW_KEY_C;
    case EKey::D: return GLFW_KEY_D;
    case EKey::E: return GLFW_KEY_E;
    case EKey::F: return GLFW_KEY_F;
    case EKey::G: return GLFW_KEY_G;
    case EKey::H: return GLFW_KEY_H;
    case EKey::I: return GLFW_KEY_I;
    case EKey::J: return GLFW_KEY_J;
    case EKey::K: return GLFW_KEY_K;
    case EKey::L: return GLFW_KEY_L;
    case EKey::M: return GLFW_KEY_M;
    case EKey::N: return GLFW_KEY_N;
    case EKey::O: return GLFW_KEY_O;
    case EKey::P: return GLFW_KEY_P;
    case EKey::Q: return GLFW_KEY_Q;
    case EKey::R: return GLFW_KEY_R;
    case EKey::S: return GLFW_KEY_S;
    case EKey::T: return GLFW_KEY_T;
    case EKey::U: return GLFW_KEY_U;
    case EKey::V: return GLFW_KEY_V;
    case EKey::W: return GLFW_KEY_W;
    case EKey::X: return GLFW_KEY_X;
    case EKey::Y: return GLFW_KEY_Y;
    case EKey::Z: return GLFW_KEY_Z;
    case EKey::LeftBracket: return GLFW_KEY_LEFT_BRACKET;
    case EKey::Backslash: return GLFW_KEY_BACKSLASH;
    case EKey::RightBracket: return GLFW_KEY_RIGHT_BRACKET;
    case EKey::GraveAccent: return GLFW_KEY_GRAVE_ACCENT;
    case EKey::World1: return GLFW_KEY_WORLD_1;
    case EKey::World2: return GLFW_KEY_WORLD_2;
    case EKey::Escape: return GLFW_KEY_ESCAPE;
    case EKey::Enter: return GLFW_KEY_ENTER;
    case EKey::Tab: return GLFW_KEY_TAB;
    case EKey::Backspace: return GLFW_KEY_BACKSPACE;
    case EKey::Insert: return GLFW_KEY_INSERT;
    case EKey::Delete: return GLFW_KEY_DELETE;
    case EKey::Right: return GLFW_KEY_RIGHT;
    case EKey::Left: return GLFW_KEY_LEFT;
    case EKey::Down: return GLFW_KEY_DOWN;
    case EKey::Up: return GLFW_KEY_UP;
    case EKey::PageUp: return GLFW_KEY_PAGE_UP;
    case EKey::PageDown: return GLFW_KEY_PAGE_DOWN;
    case EKey::Home: return GLFW_KEY_HOME;
    case EKey::End: return GLFW_KEY_END;
    case EKey::CapsLock: return GLFW_KEY_CAPS_LOCK;
    case EKey::ScrollLock: return GLFW_KEY_SCROLL_LOCK;
    case EKey::NumLock: return GLFW_KEY_NUM_LOCK;
    case EKey::PrintScreen: return GLFW_KEY_PRINT_SCREEN;
    case EKey::Pause: return GLFW_KEY_PAUSE;
    case EKey::F1: return GLFW_KEY_F1;
    case EKey::F2: return GLFW_KEY_F2;
    case EKey::F3: return GLFW_KEY_F3;
    case EKey::F4: return GLFW_KEY_F4;
    case EKey::F5: return GLFW_KEY_F5;
    case EKey::F6: return GLFW_KEY_F6;
    case EKey::F7: return GLFW_KEY_F7;
    case EKey::F8: return GLFW_KEY_F8;
    case EKey::F9: return GLFW_KEY_F9;
    case EKey::F10: return GLFW_KEY_F10;
    case EKey::F11: return GLFW_KEY_F11;
    case EKey::F12: return GLFW_KEY_F12;
    case EKey::F13: return GLFW_KEY_F13;
    case EKey::F14: return GLFW_KEY_F14;
    case EKey::F15: return GLFW_KEY_F15;
    case EKey::F16: return GLFW_KEY_F16;
    case EKey::F17: return GLFW_KEY_F17;
    case EKey::F18: return GLFW_KEY_F18;
    case EKey::F19: return GLFW_KEY_F19;
    case EKey::F20: return GLFW_KEY_F20;
    case EKey::F21: return GLFW_KEY_F21;
    case EKey::F22: return GLFW_KEY_F22;
    case EKey::F23: return GLFW_KEY_F23;
    case EKey::F24: return GLFW_KEY_F24;
    case EKey::F25: return GLFW_KEY_F25;
    case EKey::Keypad0: return GLFW_KEY_KP_0;
    case EKey::Keypad1: return GLFW_KEY_KP_1;
    case EKey::Keypad2: return GLFW_KEY_KP_2;
    case EKey::Keypad3: return GLFW_KEY_KP_3;
    case EKey::Keypad4: return GLFW_KEY_KP_4;
    case EKey::Keypad5: return GLFW_KEY_KP_5;
    case EKey::Keypad6: return GLFW_KEY_KP_6;
    case EKey::Keypad7: return GLFW_KEY_KP_7;
    case EKey::Keypad8: return GLFW_KEY_KP_8;
    case EKey::Keypad9: return GLFW_KEY_KP_9;
    case EKey::KeypadDecimal: return GLFW_KEY_KP_DECIMAL;
    case EKey::KeypadDivide: return GLFW_KEY_KP_DIVIDE;
    case EKey::KeypadMultiply: return GLFW_KEY_KP_MULTIPLY;
    case EKey::KeypadSubtract: return GLFW_KEY_KP_SUBTRACT;
    case EKey::KeypadAdd: return GLFW_KEY_KP_ADD;
    case EKey::KeypadEnter: return GLFW_KEY_KP_ENTER;
    case EKey::KeypadEqual: return GLFW_KEY_KP_EQUAL;
    case EKey::LeftShift: return GLFW_KEY_LEFT_SHIFT;
    case EKey::LeftControl: return GLFW_KEY_LEFT_CONTROL;
    case EKey::LeftAlt: return GLFW_KEY_LEFT_ALT;
    case EKey::LeftSuper: return GLFW_KEY_LEFT_SUPER;
    case EKey::RightShift: return GLFW_KEY_RIGHT_SHIFT;
    case EKey::RightControl: return GLFW_KEY_RIGHT_CONTROL;
    case EKey::RightAlt: return GLFW_KEY_RIGHT_ALT;
    case EKey::RightSuper: return GLFW_KEY_RIGHT_SUPER;
    case EKey::Menu: return GLFW_KEY_MENU;
    case EKey::Last: return GLFW_KEY_LAST;
    default: return GLFW_KEY_UNKNOWN;
    }
}

constexpr EKey GLFWKeyToKey(const int key)
{
    switch (key)
    {
    case GLFW_KEY_SPACE: return EKey::Space;
    case GLFW_KEY_APOSTROPHE: return EKey::Apostrophe;
    case GLFW_KEY_COMMA: return EKey::Comma;
    case GLFW_KEY_MINUS: return EKey::Minus;
    case GLFW_KEY_PERIOD: return EKey::Period;
    case GLFW_KEY_SLASH: return EKey::Slash;
    case GLFW_KEY_0: return EKey::Num0;
    case GLFW_KEY_1: return EKey::Num1;
    case GLFW_KEY_2: return EKey::Num2;
    case GLFW_KEY_3: return EKey::Num3;
    case GLFW_KEY_4: return EKey::Num4;
    case GLFW_KEY_5: return EKey::Num5;
    case GLFW_KEY_6: return EKey::Num6;
    case GLFW_KEY_7: return EKey::Num7;
    case GLFW_KEY_8: return EKey::Num8;
    case GLFW_KEY_9: return EKey::Num9;
    case GLFW_KEY_SEMICOLON: return EKey::Semicolon;
    case GLFW_KEY_EQUAL: return EKey::Equal;
    case GLFW_KEY_A: return EKey::A;
    case GLFW_KEY_B: return EKey::B;
    case GLFW_KEY_C: return EKey::C;
    case GLFW_KEY_D: return EKey::D;
    case GLFW_KEY_E: return EKey::E;
    case GLFW_KEY_F: return EKey::F;
    case GLFW_KEY_G: return EKey::G;
    case GLFW_KEY_H: return EKey::H;
    case GLFW_KEY_I: return EKey::I;
    case GLFW_KEY_J: return EKey::J;
    case GLFW_KEY_K: return EKey::K;
    case GLFW_KEY_L: return EKey::L;
    case GLFW_KEY_M: return EKey::M;
    case GLFW_KEY_N: return EKey::N;
    case GLFW_KEY_O: return EKey::O;
    case GLFW_KEY_P: return EKey::P;
    case GLFW_KEY_Q: return EKey::Q;
    case GLFW_KEY_R: return EKey::R;
    case GLFW_KEY_S: return EKey::S;
    case GLFW_KEY_T: return EKey::T;
    case GLFW_KEY_U: return EKey::U;
    case GLFW_KEY_V: return EKey::V;
    case GLFW_KEY_W: return EKey::W;
    case GLFW_KEY_X: return EKey::X;
    case GLFW_KEY_Y: return EKey::Y;
    case GLFW_KEY_Z: return EKey::Z;
    case GLFW_KEY_LEFT_BRACKET: return EKey::LeftBracket;
    case GLFW_KEY_BACKSLASH: return EKey::Backslash;
    case GLFW_KEY_RIGHT_BRACKET: return EKey::RightBracket;
    case GLFW_KEY_GRAVE_ACCENT: return EKey::GraveAccent;
    case GLFW_KEY_WORLD_1: return EKey::World1;
    case GLFW_KEY_WORLD_2: return EKey::World2;
    case GLFW_KEY_ESCAPE: return EKey::Escape;
    case GLFW_KEY_ENTER: return EKey::Enter;
    case GLFW_KEY_TAB: return EKey::Tab;
    case GLFW_KEY_BACKSPACE: return EKey::Backspace;
    case GLFW_KEY_INSERT: return EKey::Insert;
    case GLFW_KEY_DELETE: return EKey::Delete;
    case GLFW_KEY_RIGHT: return EKey::Right;
    case GLFW_KEY_LEFT: return EKey::Left;
    case GLFW_KEY_DOWN: return EKey::Down;
    case GLFW_KEY_UP: return EKey::Up;
    case GLFW_KEY_PAGE_UP: return EKey::PageUp;
    case GLFW_KEY_PAGE_DOWN: return EKey::PageDown;
    case GLFW_KEY_HOME: return EKey::Home;
    case GLFW_KEY_END: return EKey::End;
    case GLFW_KEY_CAPS_LOCK: return EKey::CapsLock;
    case GLFW_KEY_SCROLL_LOCK: return EKey::ScrollLock;
    case GLFW_KEY_NUM_LOCK: return EKey::NumLock;
    case GLFW_KEY_PRINT_SCREEN: return EKey::PrintScreen;
    case GLFW_KEY_PAUSE: return EKey::Pause;
    case GLFW_KEY_F1: return EKey::F1;
    case GLFW_KEY_F2: return EKey::F2;
    case GLFW_KEY_F3: return EKey::F3;
    case GLFW_KEY_F4: return EKey::F4;
    case GLFW_KEY_F5: return EKey::F5;
    case GLFW_KEY_F6: return EKey::F6;
    case GLFW_KEY_F7: return EKey::F7;
    case GLFW_KEY_F8: return EKey::F8;
    case GLFW_KEY_F9: return EKey::F9;
    case GLFW_KEY_F10: return EKey::F10;
    case GLFW_KEY_F11: return EKey::F11;
    case GLFW_KEY_F12: return EKey::F12;
    case GLFW_KEY_F13: return EKey::F13;
    case GLFW_KEY_F14: return EKey::F14;
    case GLFW_KEY_F15: return EKey::F15;
    case GLFW_KEY_F16: return EKey::F16;
    case GLFW_KEY_F17: return EKey::F17;
    case GLFW_KEY_F18: return EKey::F18;
    case GLFW_KEY_F19: return EKey::F19;
    case GLFW_KEY_F20: return EKey::F20;
    case GLFW_KEY_F21: return EKey::F21;
    case GLFW_KEY_F22: return EKey::F22;
    case GLFW_KEY_F23: return EKey::F23;
    case GLFW_KEY_F24: return EKey::F24;
    case GLFW_KEY_F25: return EKey::F25;
    case GLFW_KEY_KP_0: return EKey::Keypad0;
    case GLFW_KEY_KP_1: return EKey::Keypad1;
    case GLFW_KEY_KP_2: return EKey::Keypad2;
    case GLFW_KEY_KP_3: return EKey::Keypad3;
    case GLFW_KEY_KP_4: return EKey::Keypad4;
    case GLFW_KEY_KP_5: return EKey::Keypad5;
    case GLFW_KEY_KP_6: return EKey::Keypad6;
    case GLFW_KEY_KP_7: return EKey::Keypad7;
    case GLFW_KEY_KP_8: return EKey::Keypad8;
    case GLFW_KEY_KP_9: return EKey::Keypad9;
    case GLFW_KEY_KP_DECIMAL: return EKey::KeypadDecimal;
    case GLFW_KEY_KP_DIVIDE: return EKey::KeypadDivide;
    case GLFW_KEY_KP_MULTIPLY: return EKey::KeypadMultiply;
    case GLFW_KEY_KP_SUBTRACT: return EKey::KeypadSubtract;
    case GLFW_KEY_KP_ADD: return EKey::KeypadAdd;
    case GLFW_KEY_KP_ENTER: return EKey::KeypadEnter;
    case GLFW_KEY_KP_EQUAL: return EKey::KeypadEqual;
    case GLFW_KEY_LEFT_SHIFT: return EKey::LeftShift;
    case GLFW_KEY_LEFT_CONTROL: return EKey::LeftControl;
    case GLFW_KEY_LEFT_ALT: return EKey::LeftAlt;
    case GLFW_KEY_LEFT_SUPER: return EKey::LeftSuper;
    case GLFW_KEY_RIGHT_SHIFT: return EKey::RightShift;
    case GLFW_KEY_RIGHT_CONTROL: return EKey::RightControl;
    case GLFW_KEY_RIGHT_ALT: return EKey::RightAlt;
    case GLFW_KEY_RIGHT_SUPER: return EKey::RightSuper;
    case GLFW_KEY_MENU: return EKey::Menu;
    default: return EKey::Unknown;
    }
}
