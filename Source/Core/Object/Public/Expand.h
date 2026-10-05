#pragma once
#include <types.h>
enum class EExpand : u8
{
	None = 0x00,
	Horizontal = 0x01,
	Vertical = 0x02,
	All = Horizontal | Vertical,
};
