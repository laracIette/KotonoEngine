#pragma once
#include <kotono_common/Path.h>
#include <optional>
#include <string>
#include <vector>
struct UReflectionResult final
{
	struct TypeInfo
	{
		bool isTemplate;
		std::string name;
		std::optional<std::string> base;
	};

	struct MemberInfo
	{
		std::string type;
		std::string name;
	};

	UPath path;
	TypeInfo type;
	std::vector<MemberInfo> members;
};