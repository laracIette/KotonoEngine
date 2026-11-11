#pragma once
#include <types.h>
#include <string>

struct UVariableInfo final
{
	std::string type;
	std::string name;
	size offset;

    auto operator==(UVariableInfo const&) const -> b8 = default;
};