#pragma once
#include <types.h>
#include <optional>
#include <string>
#include <vector>

struct UReflectionResult;

class SGenerator final
{
	struct ClassInfo
	{
		struct VariableInfo
		{
			std::string type;
			std::string name;
		};

		std::string name;
		std::optional<std::string> base;
		b8 isAbstract;
		std::vector<VariableInfo> variables;
	};

public:
	static void GenerateAll();
	static void GenerateUpdated();
	static void GenerateRegistrator();

private:
	static void Generate(UReflectionResult const& reflectionResult);
	static void GenerateHeader(UReflectionResult const& reflectionResult);
	static void GenerateSource(UReflectionResult const& reflectionResult);
	 
	static ClassInfo GetClassInfo(UReflectionResult const& reflectionResult);
};
