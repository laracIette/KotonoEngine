#pragma once
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
		std::vector<VariableInfo> variables;
	};

public:
	static void GenerateAll();
	static void GenerateUpdated();

private:
	static void Generate(const UReflectionResult& reflectionResult);
	static void GenerateHeader(const UReflectionResult& reflectionResult);
	static void GenerateSource(const UReflectionResult& reflectionResult);
	 
	static ClassInfo GetClassInfo(const UReflectionResult& reflectionResult);
};
