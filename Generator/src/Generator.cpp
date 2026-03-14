#include "Generator.h"
#include <iostream>
#include <kotono_common/Path.h>
#include <kotono_io/File.h>
#include <kotono_io/FileExplorer.h>
#include <kotono_io/Serializer.h>
#include <kotono_reflection/Reflector.h>
#include <nlohmann/json.hpp>
#include <ranges>

static const UPath CorePath{ "${ENGINE_DIRECTORY}/Core" };
static const UPath RegistryPath{ CorePath / "objects.ktregistry" };

static std::string to_upper(std::string s)
{
	std::ranges::transform(s, s.begin(), [](unsigned char c)
		{
			return static_cast<char>(std::toupper(c));
		});

	return s;
}

void SGenerator::GenerateAll() const
{
	std::cout << "Clearing registry..." << std::endl;
	USerializer::Serialize(nlohmann::json::object(), RegistryPath);
	GenerateUpdated();
}

void SGenerator::GenerateUpdated() const
{
	std::cout << "Generating..." << std::endl;

	nlohmann::json json{};
	USerializer::Deserialize(json, RegistryPath);

	const auto& reflectionResults{ Reflector.GetReflectionResults() };
	for (const auto& reflectionResult : reflectionResults)
	{
		const UFile file(reflectionResult.path);

		const auto entryPath{ file.Name() };
		const auto ftime{ file.LastWriteTime() };
		const auto formattedTime{ std::format("{0:%F}-{0:%T}", ftime) };

		bool isInList{ false };
		for (auto& header : json["headers"])
		{
			if (header["path"] != entryPath)
			{
				continue;
			}

			if (header["modified"] != formattedTime)
			{
				header["modified"] = formattedTime;
				Generate(reflectionResult);
			}

			isInList = true;
			break;
		}

		if (!isInList)
		{
			nlohmann::json header{};
			header["path"] = entryPath;
			header["modified"] = formattedTime;
			json["headers"].push_back(header);
			Generate(reflectionResult);
		}
	}

	USerializer::Serialize(json, RegistryPath);
}

void SGenerator::Generate(const UReflectionResult& reflectionResult) const
{
	const auto classInfo{ GetClassInfo(reflectionResult) };
	const std::string generatedCodeHeader{ !classInfo.base.has_value()
		? std::format(
R"(#define GENERATED_{0}() \
	private: \
		using Self = {1}; \
	public: \
		virtual void SerializeTo(nlohmann::json& json) const; \
		virtual void DeserializeFrom(const nlohmann::json& json); \
		virtual std::vector<UVariableInfo> GetMemberVariables() const; \
	private: \
		UPtr<{1}> Ptr() const;
)",
			to_upper(classInfo.name),
			classInfo.name
		)
		: std::format(
R"(#define GENERATED_{0}() \
	private: \
		using Self = {1}; \
		using Base = {2}; \
		using Base::Base; \
	public: \
		void SerializeTo(nlohmann::json& json) const override; \
		void DeserializeFrom(const nlohmann::json& json) override; \
		std::vector<UVariableInfo> GetMemberVariables() const override; \
	private: \
		UPtr<{1}> Ptr() const;
)",
			to_upper(classInfo.name),
			classInfo.name,
			classInfo.base.value()
		)
	};

	const UPath fileHeader{ CorePath / "include" / "kotono_core" / "generated" / UPath(reflectionResult.path.ToPath().filename().replace_extension(".generated.h")) };
	UFile(fileHeader).WriteString(generatedCodeHeader);

	std::ostringstream objectClassHeaders;
	for (const auto& header : classInfo.headers)
	{
		objectClassHeaders << std::format(R"(#include "{0}")", header) << std::endl;
	}

	std::ostringstream serializeCode;
	for (const auto& variable : classInfo.variables)
	{
		serializeCode << std::format(R"(	serialize(json["{0}"], {0});)", variable.name) << std::endl;
	}

	std::ostringstream deserializeCode;
	for (const auto& variable : classInfo.variables)
	{
		deserializeCode << std::format(R"(	deserialize(json.at("{0}"), {0});)", variable.name) << std::endl;
	}

	std::ostringstream memberVariablesCode;
	for (const auto& variable : classInfo.variables)
	{
		memberVariablesCode << std::format(R"(		{{ "{0}", "{1}", offsetof(Self, {1}) }},)", variable.type, variable.name) << std::endl;
	}

	const std::string generatedCodeCPP{ !classInfo.base.has_value()
		? std::format(
R"(#include "{0}"
#include "Ptr.h"
#include "serialize.h"
#include <nlohmann/json.hpp>
{1}

void {2}::SerializeTo(nlohmann::json& json) const
{{
{3}
}}

void {2}::DeserializeFrom(const nlohmann::json& json)
{{
{4}
}}

std::vector<UVariableInfo> {2}::GetMemberVariables() const
{{
	return {{
{5}
	}};
}}

UPtr<{2}> {2}::Ptr() const
{{
	return static_cast<UPtrOwner<{2}>*>(ptrOwner_);
}}
)",
			reflectionResult.path.ToPath().filename().string(),
			objectClassHeaders.str(),
			classInfo.name,
			serializeCode.str(),
			deserializeCode.str(),
			memberVariablesCode.str()
		)
		: std::format(
R"(#include "{0}"
#include "Ptr.h"
#include "serialize.h"
#include <nlohmann/json.hpp>
{1}

void {2}::SerializeTo(nlohmann::json& json) const
{{
	Base::SerializeTo(json);
{3}
}}

void {2}::DeserializeFrom(const nlohmann::json& json)
{{
	Base::DeserializeFrom(json);
{4}
}}

std::vector<UVariableInfo> {2}::GetMemberVariables() const
{{
	auto result{{ Base::GetMemberVariables() }};
	result.insert(result.end(), {{
{5}
	}});
	return result;
}}

UPtr<{2}> {2}::Ptr() const
{{
	return static_cast<UPtrOwner<{2}>*>(ptrOwner_);
}}
)",
			reflectionResult.path.ToPath().filename().string(),
			objectClassHeaders.str(),
			classInfo.name,
			serializeCode.str(),
			deserializeCode.str(),
			memberVariablesCode.str()
		)
	};

	const UPath fileCPP{ CorePath / "src" / "generated" / UPath(reflectionResult.path.ToPath().filename().replace_extension(".generated.cpp")) };
	UFile(fileCPP).WriteString(generatedCodeCPP);

	std::cout << "Generated " << reflectionResult.path.ToString() << std::endl;
}

SGenerator::ClassInfo SGenerator::GetClassInfo(const UReflectionResult& reflectionResult) const
{
	std::vector<ClassInfo::VariableInfo> variables;
	std::ranges::copy(
		reflectionResult.members
		| std::views::transform([](const UReflectionResult::MemberInfo& member) { return ClassInfo::VariableInfo{ member.type, member.name }; }),
		std::back_inserter(variables)
	);

	std::vector<std::string> headers;
	std::ranges::copy(
		reflectionResult.fwdClasses
		| std::views::filter([](const std::string& name) { return Reflector.IsObjectType(name); })
		| std::views::transform([](const std::string& name) { return std::format("{}.h", name.substr(1)); }),
		std::back_inserter(headers)
	);

	return {
		.name = reflectionResult.type.name,
		.base = reflectionResult.type.base,
		.variables = variables,
		.headers = headers,
	};
}
