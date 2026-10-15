#pragma once
#include <Path/Path.h>
#include <string>
#include <vector>

struct UReflectionResult final
{
	struct TypeInfo
	{
		std::string name;
		std::string base;
		b8 isAbstract;
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