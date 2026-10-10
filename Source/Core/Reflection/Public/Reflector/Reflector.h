#pragma once
#include "ReflectionResult.h"
#include <span>
#include <types.h>
#include <vector>

class GReflector final
{
public:
	void Reflect();

	auto GetReflectionResults() const -> std::span<UReflectionResult const>;

	auto IsObjectType(UReflectionResult::TypeInfo const& type) const -> b8;

private:
	auto GetTypeInfo(std::string const& content) const -> UReflectionResult::TypeInfo;
	auto GetMemberInfos(std::string const& content) const -> std::vector<UReflectionResult::MemberInfo>;

private:
	std::vector<UReflectionResult> allResults_;
	std::vector<UReflectionResult> reflectionResults_;
};

inline GReflector Reflector;
