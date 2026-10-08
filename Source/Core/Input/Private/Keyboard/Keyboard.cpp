#include "Keyboard/Keyboard.h"

#include <functional>
#include <GLFW/glfw3.h>
#include <enum_utils.h>
#include <Logging/log.h>
#include <Window/Window.h>
#include <Containers/Map.h>

#define KT_LOG_IMPORTANCE_LEVEL_KEYBOARD ELogImportance::Low

static UMap<GLFWwindow*, std::function<void(EKey, i32)>> KeyCallbacks{};

namespace
{
	struct ModifierKeys final
	{
		EModifier modifier;
		EKey primary, secondary;
	};
}

static constexpr std::array MODIFIER_KEYS{
	ModifierKeys{ EModifier::Shift,     EKey::LeftShift,      EKey::RightShift      },
	ModifierKeys{ EModifier::Control,   EKey::LeftControl,    EKey::RightControl    },
	ModifierKeys{ EModifier::Alt,       EKey::LeftAlt,        EKey::RightAlt        },
	ModifierKeys{ EModifier::Super,     EKey::LeftSuper,      EKey::RightSuper      },
	ModifierKeys{ EModifier::NumLock,   EKey::NumLock,        EKey::Unknown         },
	ModifierKeys{ EModifier::CapsLock,  EKey::CapsLock,       EKey::Unknown         },
};

static constexpr auto GLFWKeyToKey(i32 key) noexcept -> EKey;

static void key_callback_(GLFWwindow* window, i32 key, i32 scancode, i32 action, i32 mods)
{
    if (action == GLFW_REPEAT)
    {
        return;
    }

    auto const it{ KeyCallbacks.Find(window) };
    if (KeyCallbacks.IsValidIterator(it) && it->second)
    {
        it->second(GLFWKeyToKey(key), action);
    }
}

UKeyboard::UKeyboard(UWindow& window)
	: window_{window}
	, eventKey_{}
	, modifier_{}
	, keyStates_{}
{
}

void UKeyboard::Init()
{
    glfwSetKeyCallback(window_.GetGLFWWindow(), key_callback_);
    KeyCallbacks.TryEmplace(window_.GetGLFWWindow(), [this](EKey key, i32 action) { UpdateKey(key, action); });
}

void UKeyboard::Cleanup() const
{
    KeyCallbacks.Remove(window_.GetGLFWWindow());
}

void UKeyboard::Update()
{
    modifier_ = EModifier::None;
    for (auto const& [mod, k1, k2] : MODIFIER_KEYS)
    {
        if (GetIsKeyState(k1, EInputState::Down)
         || (k2 != EKey::Unknown && GetIsKeyState(k2, EInputState::Down)))
        {
            modifier_ |= mod;
        }
    }

    for (size key{ 0 }; key < KEY_COUNT; ++key)
    {
        for (size inputState{ 0 }; inputState < INPUT_STATE_COUNT; ++inputState)
        {
            if (GetIsKeyState(key, inputState))
            {
                eventKey_.Broadcast(static_cast<EKey>(key), static_cast<EInputState>(inputState));
            }
        }

        if (GetIsKeyState(key, EInputState::Pressed))
        {
            SetIsKeyState(key, EInputState::Pressed, false);
        }
        else if (GetIsKeyState(key, EInputState::Released))
        {
            SetIsKeyState(key, EInputState::Released, false);
        }
    }
}

void UKeyboard::UpdateKey(EKey key, i32 action)
{
    switch (action)
    {
    case GLFW_PRESS:
    {
		KT_LOG(KT_LOG_IMPORTANCE_LEVEL_KEYBOARD, "Input", "GLFW_PRESS key {0}", std::to_underlying(key));

        SetIsKeyState(key, EInputState::Released, false);

        SetIsKeyState(key, EInputState::Pressed, true);
        SetIsKeyState(key, EInputState::Down, true);
        break;
    }
    case GLFW_RELEASE:
    {
        KT_LOG(KT_LOG_IMPORTANCE_LEVEL_KEYBOARD, "Input", "GLFW_RELEASE key {0}", std::to_underlying(key));

        SetIsKeyState(key, EInputState::Pressed, false);
        SetIsKeyState(key, EInputState::Down, false);

        SetIsKeyState(key, EInputState::Released, true);
        break;
    }
    default:
        break;
    }
}

constexpr auto GLFWKeyToKey(i32 key) noexcept -> EKey
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
