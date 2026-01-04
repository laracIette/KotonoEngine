#pragma once
#include <optional>
#include <string>
#include <vector>
struct UReflectionResult;
class SGenerator final
{
	struct ClassInfo
	{
		std::string name;
		std::optional<std::string> base;
		std::vector<std::string> variables;
		std::vector<std::string> headers;
	};

public:
	void GenerateAll() const;
	void GenerateUpdated() const;

private:
	void Generate(const UReflectionResult& reflectionResult) const;
	
	ClassInfo GetClassInfo(const UReflectionResult& reflectionResult) const;
};

inline SGenerator Generator;
