#pragma once
#include <kotono_common/types.h>
enum class EKey : u8
{
	Space = 0,
	Apostrophe,
	Comma,
	Minus,
	Period,
	Slash,
	Num0,
	Num1,
	Num2,
	Num3,
	Num4,
	Num5,
	Num6,
	Num7,
	Num8,
	Num9,
	Semicolon,
	Equal,
	A,
	B,
	C,
	D,
	E,
	F,
	G,
	H,
	I,
	J,
	K,
	L,
	M,
	N,
	O,
	P,
	Q,
	R,
	S,
	T,
	U,
	V,
	W,
	X,
	Y,
	Z,
	LeftBracket,
	Backslash,
	RightBracket,
	GraveAccent,
	World1,
	World2,
	Escape,
	Enter,
	Tab,
	Backspace,
	Insert,
	Delete,
	Right,
	Left,
	Down,
	Up,
	PageUp,
	PageDown,
	Home,
	End,
	CapsLock,
	ScrollLock,
	NumLock,
	PrintScreen,
	Pause,
	F1,
	F2,
	F3,
	F4,
	F5,
	F6,
	F7,
	F8,
	F9,
	F10,
	F11,
	F12,
	F13,
	F14,
	F15,
	F16,
	F17,
	F18,
	F19,
	F20,
	F21,
	F22,
	F23,
	F24,
	F25,
	Keypad0,
	Keypad1,
	Keypad2,
	Keypad3,
	Keypad4,
	Keypad5,
	Keypad6,
	Keypad7,
	Keypad8,
	Keypad9,
	KeypadDecimal,
	KeypadDivide,
	KeypadMultiply,
	KeypadSubtract,
	KeypadAdd,
	KeypadEnter,
	KeypadEqual,
	LeftShift,
	LeftControl,
	LeftAlt,
	LeftSuper,
	RightShift,
	RightControl,
	RightAlt,
	RightSuper,
	Menu,
	Last,
	Unknown = UINT8_MAX
};

inline constexpr size KeyCount{ 121 };

inline constexpr size to_index(const EKey key)
{
	return static_cast<size>(key);
}

inline constexpr char keyToChar(const EKey key)
{
	switch (key)
	{
	case EKey::Space: return ' ';
	case EKey::A: return 'a';
	case EKey::B: return 'b';
	case EKey::C: return 'c';
	case EKey::D: return 'd';
	case EKey::E: return 'e';
	case EKey::F: return 'f';
	case EKey::G: return 'g';
	case EKey::H: return 'h';
	case EKey::I: return 'i';
	case EKey::J: return 'j';
	case EKey::K: return 'k';
	case EKey::L: return 'l';
	case EKey::M: return 'm';
	case EKey::N: return 'n';
	case EKey::O: return 'o';
	case EKey::P: return 'p';
	case EKey::Q: return 'q';
	case EKey::R: return 'r';
	case EKey::S: return 's';
	case EKey::T: return 't';
	case EKey::U: return 'u';
	case EKey::V: return 'v';
	case EKey::W: return 'w';
	case EKey::X: return 'x';
	case EKey::Y: return 'y';
	case EKey::Z: return 'z';
	default: return -1;
	}
}
