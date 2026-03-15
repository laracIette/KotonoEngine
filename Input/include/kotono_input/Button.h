#pragma once
#include <kotono_common/types.h>
enum class EButton : u8
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
	Previous = Button4,
	Next = Button5,
	Last = Button8
};

inline constexpr size ButtonCount{ 8 };

inline constexpr size to_index(const EButton button)
{
	return static_cast<size>(button);
}
