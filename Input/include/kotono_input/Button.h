#pragma once
#include <cstdint>
enum class EButton : uint8_t
{
	Button1 = 0,
	Button2 = 1,
	Button3 = 2,
	Button4 = 3,
	Button5 = 4,
	Button6 = 5,
	Button7 = 6,
	Button8 = 7,
	Left = Button1,
	Right = Button2,
	Middle = Button3,
	Last = Button8
};

inline constexpr size_t ButtonCount{ 8 };

inline constexpr size_t to_index(const EButton button)
{
	return static_cast<size_t>(button);
}
