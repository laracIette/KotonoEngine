#include "Reflector.h"
#include <kotono_common/Path.h>
#include <kotono_io/File.h>
#include <kotono_io/FileExplorer.h>
#include <regex>

void SReflector::Reflect()
{
	const auto files{ UFileExplorer("${ENGINE_DIRECTORY}/Core/include/kotono_core").GetFiles() };
	for (const auto& file : files)
	{
		const auto content{ file.ReadString() };

		const UReflectionResult reflectionResult{
			.path = file.Path().string(),
			.type = GetTypeInfo(content),
			.members = GetMemberInfos(content),
			.fwdClasses = GetForwardDeclarations(content),
		};

		reflectionResults_.push_back(reflectionResult);
	}

	std::erase_if(reflectionResults_, 
		[this](const UReflectionResult& reflectionResult)
		{
			return !IsObjectType(reflectionResult.type);
		}
	);
}

const std::vector<UReflectionResult>& SReflector::GetReflectionResults() const
{
	return reflectionResults_;
}

std::optional<UReflectionResult> SReflector::GetReflectionResult(const std::string& typeName) const
{
	const auto it{ std::find_if(reflectionResults_.begin(), reflectionResults_.end(),
		[typeName](const UReflectionResult& reflectionResult)
		{
			return reflectionResult.type.name == typeName;
		}
	) };

	if (it == reflectionResults_.end())
	{
		return std::nullopt;
	}

	return *it;
}

UReflectionResult::TypeInfo SReflector::GetTypeInfo(const std::string& content) const
{
	const std::regex pattern(R"((?:class|struct)\s+([a-zA-Z_]\w*)\s*(?::\s*(?:public|protected|private)?\s*([a-zA-Z_]\w*))?\s*\{)");
	
	std::string name{ "" };
	std::optional<std::string> base{ std::nullopt };

	std::smatch match;

	if (std::regex_search(content, match, pattern))
	{
		name = match[1].str();
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

std::vector<UReflectionResult::MemberInfo> SReflector::GetMemberInfos(const std::string& content) const
{
	std::vector<UReflectionResult::MemberInfo> result{};

	const std::regex varRegex(R"(SERIALIZE\s*\(?\s*\)?\s*(.+?)\s+([A-Za-z_]\w*)\s*;)");

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

std::vector<std::string> SReflector::GetForwardDeclarations(const std::string& content) const
{
	std::vector<std::string> result{};

	const std::regex fwdRegex(R"(\bclass\s+(?:\w+\s+)*([A-Za-z_]\w*)\s*;)");

	for (std::sregex_iterator it(content.begin(), content.end(), fwdRegex), end; it != end; ++it)
	{
		result.push_back((*it)[1]);
	}

	return result;
}

bool SReflector::IsObjectType(const UReflectionResult::TypeInfo& type) const
{
	if (type.name == "KObject")
	{
		return true;
	}

	if (type.base == std::nullopt)
	{
		return false;
	}

	const auto it{ std::find_if(reflectionResults_.begin(), reflectionResults_.end(),
		[type](const UReflectionResult& reflectionResult)
		{
			return reflectionResult.type.name == type.base;
		}
	) };

	if (it == reflectionResults_.end())
	{
		return false;
	}

	return IsObjectType(it->type);
}

bool SReflector::IsObjectType(const std::string& type) const
{
	const auto it{ std::find_if(reflectionResults_.begin(), reflectionResults_.end(),
		[type](const UReflectionResult& reflectionResult)
		{
			return reflectionResult.type.name == type;
		}
	) };

	if (it == reflectionResults_.end())
	{
		return false;
	}

	return IsObjectType(it->type);
}
