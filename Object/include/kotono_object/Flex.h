#pragma once
#include <kotono_common/types.h>
enum class EFlex : u8
{
	None = 0x00,
	Horizontal = 0x01,
	Vertical = 0x02,
	All = Horizontal | Vertical,
};

