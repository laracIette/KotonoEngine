#pragma once
#include <kotono_common/types.h>
#include <string>
struct UVariableInfo final
{
	std::string type;
	std::string name;
	size offset;
};