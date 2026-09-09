#include "Reflector.h"

#include <kotono_common/Path.h>
#include <kotono_io/File.h>
#include <kotono_io/Serializer.h>
#include <nlohmann/json.hpp>
#include <ranges>
#include <regex>

void GReflector::Reflect()
{
	nlohmann::json json{};
	const UPath includePath{ "${ENGINE_DIRECTORY}/Reflection/include.ktregistry" };
	SSerializer::Deserialize(json, includePath);

	for (const auto& file : json.at("files"))
	{
		const UPath filePath{ file };
		const auto content{ UFile{ filePath }.ReadString() };

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

const std::vector<UReflectionResult>& GReflector::GetReflectionResults() const
{
	return reflectionResults_;
}

UReflectionResult::TypeInfo GReflector::GetTypeInfo(std::string const& content) const
{
	std::regex const pattern{ R"((?:class)\s+([a-zA-Z_]\w*)\s*(?:final)?\s*(?::\s*(?:public|protected|private)?\s*([a-zA-Z_]\w*)\s*(?:<[^\{]*>)?\s*)?\{)" };
	
	std::string name{ "" };
	std::optional<std::string> base{ std::nullopt };

	std::smatch match;
	if (std::regex_search(content, match, pattern))
	{
		if (match[1].matched)
		{
			name = match[1].str();
		}
		if (match[2].matched)
		{
			base = match[2].str();
		}
	}

	return {
		.name = name,
		.base = base,
	};
}

std::vector<UReflectionResult::MemberInfo> GReflector::GetMemberInfos(const std::string& content) const
{
	std::vector<UReflectionResult::MemberInfo> result{};

	std::regex const varRegex{ R"(SERIALIZE\s*(?:\(\s*\))?\s*(?:(?:Writable|Readonly)Property\s*\(\s*)?([A-Za-z_][\w:]*(?:<[\w:\s<>,]+>)?)\s*(?:,\s*|\s+)([A-Za-z_]\w*)[^;]*;)" };

	for (std::sregex_iterator it{ content.begin(), content.end(), varRegex }, end; it != end; ++it)
	{
		const auto type{ (*it)[1].str() };
		const auto name{ (*it)[2].str() };

		result.push_back({ 
			.type = type,
			.name = name,
		});
	}

	return result;
}

bool GReflector::IsObjectType(const UReflectionResult::TypeInfo& type) const
{
	if (type.name == "KObject")
	{
		return true;
	}

	if (type.base == std::nullopt)
	{
		return false;
	}

	const auto it{ std::find_if(allResults_.begin(), allResults_.end(),
		[type](const UReflectionResult& reflectionResult)
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
