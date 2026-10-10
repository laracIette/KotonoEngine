#include "Reflector/Reflector.h"

#include <File/File.h>
#include <nlohmann/json.hpp>
#include <Path/Path.h>
#include <ranges>
#include <regex>
#include <Serializer/Serializer.h>

void GReflector::Reflect()
{
	nlohmann::json json{};
	UPath const reflectPath{ "${ENGINE_DIRECTORY}/reflect.ktregistry" };
	SSerializer::Deserialize(json, reflectPath);

	for (auto const& file : json.at("files"))
	{
		UPath const filePath{ file.get<std::string>() };
		auto const content{ UFile{ filePath }.ReadString() };

		const UReflectionResult reflectionResult{
			.path = filePath,
			.type = GetTypeInfo(content),
			.members = GetMemberInfos(content),
		};

		allResults_.push_back(reflectionResult);
	}

	reflectionResults_ = allResults_
		| std::views::filter([this](UReflectionResult const& reflectionResult) {
			return IsObjectType(reflectionResult.type);
		})
		| std::ranges::to<std::vector>();
}

auto GReflector::GetReflectionResults() const -> std::span<UReflectionResult const>
{
	return reflectionResults_;
}

auto GReflector::GetTypeInfo(std::string const& content) const -> UReflectionResult::TypeInfo
{
	std::regex const pattern{ R"((ABSTRACT\s+)?class\s+([a-zA-Z_]\w*)\s*(?:final)?\s*(?::\s*public\s+([a-zA-Z_]\w*)\s*(?:<[^>]*>)?(?:\s*,\s*[^{]+)?)?\{)" };
	
	UReflectionResult::TypeInfo typeInfo{};

	std::smatch match;
	if (std::regex_search(content, match, pattern))
	{
		if (match[1].matched)
		{
			typeInfo.isAbstract = true;
		}
		if (match[2].matched)
		{
			typeInfo.name = match[2].str();
		}
		if (match[3].matched)
		{
			typeInfo.base = match[3].str();
		}
	}

	return typeInfo;
}

auto GReflector::GetMemberInfos(std::string const& content) const -> std::vector<UReflectionResult::MemberInfo>
{
	std::vector<UReflectionResult::MemberInfo> result{};

	std::regex const varRegex{ R"(SERIALIZE\s*(?:\(\s*\))?\s*(?:(?:Writable|Readonly)Property\s*\(\s*)?([A-Za-z_][\w:]*(?:<[\w:\s<>,]+>)?)\s*(?:,\s*|\s+)([A-Za-z_]\w*)[^;]*;)" };

	for (std::sregex_iterator it{ content.begin(), content.end(), varRegex }, end; it != end; ++it)
	{
		auto const type{ (*it)[1].str() };
		auto const name{ (*it)[2].str() };

		result.push_back({ 
			.type = type,
			.name = name,
		});
	}

	return result;
}

auto GReflector::IsObjectType(const UReflectionResult::TypeInfo& type) const -> b8
{
	if (type.name == "KObject")
	{
		return true;
	}

	if (type.base == std::nullopt)
	{
		return false;
	}

	auto const it{ std::ranges::find_if(allResults_,
		[type](UReflectionResult const& reflectionResult)
		{
			return reflectionResult.type.name == type.base;
		}
	) };

	if (it == allResults_.end())
	{
		return false;
	}

	return IsObjectType(it->type);
}
