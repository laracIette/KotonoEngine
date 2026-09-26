#include "Generator.h"
#include <iostream>
#include <kotono_common/log.h>
#include <kotono_common/Path.h>
#include <kotono_io/File.h>
#include <kotono_io/Serializer.h>
#include <kotono_reflection/Reflector.h>
#include <nlohmann/json.hpp>
#include <ranges>

static const UPath RegistryPath{ "${ENGINE_DIRECTORY}/Generator/generated.ktregistry" };

static std::string to_upper(std::string s)
{
	std::ranges::transform(
		s, s.begin(), [](unsigned char c)
		{
			return static_cast<char>(std::toupper(c));
		}
	);

	return s;
}

void SGenerator::GenerateAll()
{
	KT_LOG(ELogImportanceLevel::High, "Generator", "Clearing registry...");

	SSerializer::Serialize(nlohmann::json::object(), RegistryPath);
	GenerateUpdated();
}

void SGenerator::GenerateUpdated()
{
	KT_LOG(ELogImportanceLevel::High, "Generator", "Generating...");

	nlohmann::json json{};
	SSerializer::Deserialize(json, RegistryPath);

	auto const& reflectionResults{ Reflector.GetReflectionResults() };
	for (auto const& reflectionResult : reflectionResults)
	{
		const UFile file{ reflectionResult.path };

		auto const entryPath{ reflectionResult.path.ToString() };
		auto const ftime{ file.LastWriteTime() };
		auto const formattedTime{ std::format("{0:%F}-{0:%T}", ftime) };

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

	SSerializer::Serialize(json, RegistryPath);
}

void SGenerator::GenerateRegistrator()
{
	std::ostringstream functionsCode;
	std::ostringstream callCode;

	auto const& reflectionResults{ Reflector.GetReflectionResults() };
	for (auto const& reflectionResult : reflectionResults)
	{
		functionsCode << std::format("extern void Register_{0}();", reflectionResult.type.name) << std::endl;
		callCode << std::format("	Register_{0}();", reflectionResult.type.name) << std::endl;
	}

	const std::string generatedCode{ 
		std::format(
R"({0}
void RegisterObjectClasses()
{{
{1}
}}
)",
			functionsCode.str(),
			callCode.str()
		)
	};

	UPath const filePath{ "${ENGINE_DIRECTORY}/Application/src/generated/ClassRegistrator.generated.inl" };
	UFile{ filePath }.WriteString(generatedCode);

	KT_LOG(ELogImportanceLevel::High, "Generator", "Generated class registrator");
}

void SGenerator::Generate(const UReflectionResult& reflectionResult)
{
	GenerateHeader(reflectionResult);
	GenerateSource(reflectionResult);

	KT_LOG(ELogImportanceLevel::High, "Generator", "Generated {0}", reflectionResult.path.ToPath().string());
}

void SGenerator::GenerateHeader(const UReflectionResult& reflectionResult)
{
	auto const classInfo{ GetClassInfo(reflectionResult) };

	std::string const generatedCode{ !classInfo.base.has_value()
		? std::format(
R"(#pragma once
#define GENERATED_{0}() \
	private: \
		using Self = {1}; \
	public: \
		virtual void SerializeTo(nlohmann::json& json) const; \
		virtual void DeserializeFrom(const nlohmann::json& json); \
		virtual std::vector<UVariableInfo> GetMemberVariables() const; \
		UPtr<{1}> Ptr() const;
)",
			to_upper(classInfo.name),
			classInfo.name
		)
		: std::format(
R"(#pragma once
#define GENERATED_{0}() \
	private: \
		using Self = {1}; \
		using Base = {2}; \
		using Base::Base; \
	public: \
		void SerializeTo(nlohmann::json& json) const override; \
		void DeserializeFrom(const nlohmann::json& json) override; \
		std::vector<UVariableInfo> GetMemberVariables() const override; \
		UPtr<{1}> Ptr() const;
)",
			to_upper(classInfo.name),
			classInfo.name,
			classInfo.base.value()
		)
	};

	UPath const fileDirectory{ reflectionResult.path.Directory() };
	UPath const fileName{ reflectionResult.path.ToPath().filename().replace_extension(".generated.h") };
	UFile{ fileDirectory / "generated" / fileName }.WriteString(generatedCode);
}

void SGenerator::GenerateSource(const UReflectionResult& reflectionResult)
{
	auto const classInfo{ GetClassInfo(reflectionResult) };

	std::ostringstream serializeCode;
	for (auto const& variable : classInfo.variables)
	{
		serializeCode << std::format(R"(	USerialize<decltype({0})>{{}}(get(json, "{0}"), {0});)", variable.name) << std::endl;
	}

	std::ostringstream deserializeCode;
	for (auto const& variable : classInfo.variables)
	{
		deserializeCode << std::format(R"(	if (contains(json, "{0}")) UDeserialize<decltype({0})>{{}}(get(json, "{0}"), {0});)", variable.name) << std::endl;
	}

	std::ostringstream memberVariablesCode;
	for (auto const& variable : classInfo.variables)
	{
		memberVariablesCode << std::format(R"(		{{ "{0}", "{1}", offsetof(Self, {1}) }},)", variable.type, variable.name) << std::endl;
	}

	const std::string generatedCode{ !classInfo.base.has_value()
		? std::format(
R"(void Register_{0}() 
{{
	UAutoRegister{{ "{0}", []() {{ return UCreate<{0}>{{}}(); }} }};
}}

void {0}::SerializeTo(nlohmann::json& json) const
{{
{1}
}}

void {0}::DeserializeFrom(const nlohmann::json& json)
{{
{2}
}}

std::vector<UVariableInfo> {0}::GetMemberVariables() const
{{
	return {{
{3}
	}};
}}

UPtr<{0}> {0}::Ptr() const
{{
	return UPtr<{0}>{{ ptr_ }};
}}
)",
			classInfo.name,
			serializeCode.str(),
			deserializeCode.str(),
			memberVariablesCode.str()
		)
		: std::format(
R"(void Register_{0}() 
{{
	UAutoRegister{{ "{0}", []() {{ return UCreate<{0}>{{}}(); }} }};
}}

void {0}::SerializeTo(nlohmann::json& json) const
{{
	Base::SerializeTo(json);
{1}
}}

void {0}::DeserializeFrom(const nlohmann::json& json)
{{
	Base::DeserializeFrom(json);
{2}
}}

std::vector<UVariableInfo> {0}::GetMemberVariables() const
{{
	auto result{{ Base::GetMemberVariables() }};
	result.insert(result.end(), {{
{3}
	}});
	return result;
}}

UPtr<{0}> {0}::Ptr() const
{{
	return UPtr<{0}>{{ ptr_ }};
}}
)",
			classInfo.name,
			serializeCode.str(),
			deserializeCode.str(),
			memberVariablesCode.str()
		)
	};

	const UPath fileDirectory{ reflectionResult.path.Directory().Directory().Directory() / "src" };
	const UPath fileName{ reflectionResult.path.ToPath().filename().replace_extension(".generated.inl") };
	UFile{ fileDirectory / "generated" / fileName }.WriteString(generatedCode);
}

SGenerator::ClassInfo SGenerator::GetClassInfo(const UReflectionResult& reflectionResult)
{
	std::vector<ClassInfo::VariableInfo> variables;
	std::ranges::copy(
		reflectionResult.members
		| std::views::transform([](const UReflectionResult::MemberInfo& member) { return ClassInfo::VariableInfo{ member.type, member.name }; })
		, std::back_inserter(variables)
	);

	return {
		.name = reflectionResult.type.name,
		.base = reflectionResult.type.base,
		.variables = variables,
	};
}
