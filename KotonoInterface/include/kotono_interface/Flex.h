#pragma once
enum class EFlex : char
{
	None = 0x00,
	Horizontal = 0x01,
	Vertical = 0x02,
	All = Horizontal | Vertical,
};

inline constexpr EFlex operator&(EFlex first, EFlex second)
{
	return static_cast<EFlex>(static_cast<char>(first) & static_cast<char>(second));
}

inline constexpr EFlex operator|(EFlex first, EFlex second)
{
	return static_cast<EFlex>(static_cast<char>(first) | static_cast<char>(second));
}
