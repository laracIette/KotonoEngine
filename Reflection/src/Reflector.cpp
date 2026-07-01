#include "Reflector.h"
#include <kotono_common/Path.h>
#include <kotono_io/File.h>
#include <kotono_io/Serializer.h>
#include <nlohmann/json.hpp>
#include <regex>

void GReflector::Reflect()
{
	nlohmann::json json{};
	const UPath includePath{ "${ENGINE_DIRECTORY}/Reflection/include.ktregistry" };
	SSerializer::Deserialize(json, includePath);

	for (const auto& file : json.at("files"))
	{
		const UPath filePath{ file };
		const auto content{ UFile(filePath).ReadString() };

		const UReflectionResult reflectionResult{
			.path = filePath,
			.type = GetTypeInfo(content),
			.members = GetMemberInfos(content),
		};

		allResults_.push_back(reflectionResult);
	}

	reflectionResults_ = allResults_;

	std::erase_if(reflectionResults_, 
		[this](const UReflectionResult& reflectionResult)
		{
			if (reflectionResult.type.isTemplate || !IsObjectType(reflectionResult.type))
			{
				return true;
			}
			return reflectionResult.type.isTemplate || !IsObjectType(reflectionResult.type);
		}
	);
}

const std::vector<UReflectionResult>& GReflector::GetReflectionResults() const
{
	return reflectionResults_;
}

UReflectionResult::TypeInfo GReflector::GetTypeInfo(const std::string& content) const
{
	const std::regex pattern(R"((template\s*<[^;\{]*>\s+)?(?:class)\s+([a-zA-Z_]\w*)\s*(?:final)?\s*(?::\s*(?:public|protected|private)?\s*([a-zA-Z_]\w*)\s*(?:<[^\{]*>)?\s*)?\{)");
	
	bool isTemplate{ false };
	std::string name{ "" };
	std::optional<std::string> base{ std::nullopt };

	std::smatch match;
	if (std::regex_search(content, match, pattern))
	{
		isTemplate = match[1].matched;
		if (match[2].matched)
		{
			name = match[2].str();
		}
		if (match[3].matched)
		{
			base = match[3].str();
		}
	}

	return {
		.isTemplate = isTemplate,
		.name = name,
		.base = base,
	};
}

std::vector<UReflectionResult::MemberInfo> GReflector::GetMemberInfos(const std::string& content) const
{
	std::vector<UReflectionResult::MemberInfo> result{};

	const std::regex varRegex(R"(SERIALIZE\s*(?:\(\s*\))?\s*(?:(?:Writable|Readonly)Property\s*\(\s*)?(.*?)(?:\s*,\s*|\s+)([A-Za-z_]\w*)\s*(?:,\s*[A-Za-z_]\w*\s*\)\s*)?;)");
	//const std::regex varRegex(R"(SERIALIZE\s*\(?\s*\)?\s*(.+?)\s+([A-Za-z_]\w*)\s*;)");

	for (std::sregex_iterator it(content.begin(), content.end(), varRegex), end; it != end; ++it)
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
