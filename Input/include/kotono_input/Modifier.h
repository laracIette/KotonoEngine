#pragma once
#include <kotono_common/types.h>

enum class EModifier : u8
{
	None		= 0b0000'0000,
	Shift		= 0b0000'0001,
	Control		= 0b0000'0010,
	Alt			= 0b0000'0100,
	Super		= 0b0000'1000,
	CapsLock	= 0b0001'0000,
	NumLock		= 0b0010'0000
};
