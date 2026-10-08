#pragma once
#include "types.h"

enum class ELogImportance : u8
{
	None,
	Low,
	Medium,
	High
};

#ifndef KT_LOG_COMPILE_TIME_LEVEL
#define KT_LOG_COMPILE_TIME_LEVEL ELogImportance::High
#endif

#define KT_SHOULD_LOG(level) ((level) >= KT_LOG_COMPILE_TIME_LEVEL)
