#pragma once
#include "ReflectionResult.h"
#include <vector>
class SReflector final
{
public:
	void Reflect();

	const std::vector<UReflectionResult>& GetReflectionResults() const;
	std::optional<UReflectionResult> GetReflectionResult(const std::string& typeName) const;

	bool IsObjectType(const UReflectionResult::TypeInfo& type) const;
	bool IsObjectType(const std::string& type) const;

private:
	UReflectionResult::TypeInfo GetTypeInfo(const std::string& content) const;
	std::vector<UReflectionResult::MemberInfo> GetMemberInfos(const std::string& content) const;
	std::vector<std::string> GetForwardDeclarations(const std::string& content) const;

private:
	std::vector<UReflectionResult> reflectionResults_;
};

inline SReflector Reflector;
