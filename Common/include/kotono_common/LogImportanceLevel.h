#pragma once
#include <kotono_common/types.h>
enum class ELogImportanceLevel : u8
{
	None,
	Low,
	Medium,
	High
};

#ifndef KT_LOG_COMPILE_TIME_LEVEL
#define KT_LOG_COMPILE_TIME_LEVEL ELogImportanceLevel::High
#endif

#define KT_SHOULD_LOG(level) ((level) >= KT_LOG_COMPILE_TIME_LEVEL)
