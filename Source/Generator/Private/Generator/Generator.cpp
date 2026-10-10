#include "Generator.h"

#include <File/File.h>
#include <iostream>
#include <Logging/log.h>
#include <nlohmann/json.hpp>
#include <Path/Path.h>
#include <ranges>
#include <Reflector/Reflector.h>
#include <Serializer/Serializer.h>

static const UPath RegistryPath{ "${ENGINE_DIRECTORY}/Cache/Generator/generated.ktregistry" };
static const UPath GeneratedPath{ "${ENGINE_DIRECTORY}/Cache/Generator/Generated" };

void SGenerator::GenerateAll()
{
	KT_LOG(ELogImportance::High, "Generator", "Clearing registry...");

	SSerializer::Serialize(nlohmann::json::object(), RegistryPath);
	GenerateUpdated();
}

void SGenerator::GenerateUpdated()
{
	KT_LOG(ELogImportance::High, "Generator", "Generating...");

	nlohmann::json json{};
	SSerializer::Deserialize(json, RegistryPath);

	for (auto const& reflectionResult : Reflector.GetReflectionResults())
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

	for (auto const& reflectionResult : Reflector.GetReflectionResults())
	{
		if (!reflectionResult.type.isAbstract)
		{
			functionsCode << std::format("extern void Register_{0}();", reflectionResult.type.name) << std::endl;
			callCode << std::format("	Register_{0}();", reflectionResult.type.name) << std::endl;
		}
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

	UPath const filePath{ GeneratedPath / "ClassRegistrator.generated.inl" };
	UFile{ filePath }.WriteString(generatedCode);

	KT_LOG(ELogImportance::High, "Generator", "Generated class registrator");
}

void SGenerator::Generate(UReflectionResult const& reflectionResult)
{
	GenerateHeader(reflectionResult);
	GenerateSource(reflectionResult);

	KT_LOG(ELogImportance::High, "Generator", "Generated {0}", reflectionResult.path.ToPath().string());
}

void SGenerator::GenerateHeader(UReflectionResult const& reflectionResult)
{
	auto const classInfo{ GetClassInfo(reflectionResult) };

	std::string const generatedCode{ !classInfo.base.has_value()
		? std::format(
R"(#pragma once

#ifdef GENERATED
#undef GENERATED
#endif

#define GENERATED() \
	private: \
		using Self = {0}; \
	public: \
		virtual void SerializeTo(nlohmann::json& json) const; \
		virtual void DeserializeFrom(const nlohmann::json& json); \
		virtual std::vector<UVariableInfo> GetMemberVariables() const; \
		UPtr<{0}> Ptr() const; \
	private:
)",
			classInfo.name
		)
		: std::format(
R"(#pragma once

#ifdef GENERATED
#undef GENERATED
#endif

#define GENERATED() \
	private: \
		using Self = {0}; \
		using Base = {1}; \
		using Base::Base; \
	public: \
		void SerializeTo(nlohmann::json& json) const override; \
		void DeserializeFrom(const nlohmann::json& json) override; \
		std::vector<UVariableInfo> GetMemberVariables() const override; \
		UPtr<{0}> Ptr() const; \
	private:
)",
			classInfo.name,
			classInfo.base.value()
		)
	};

	UPath const fileName{ reflectionResult.path.ToPath().filename().replace_extension(".generated.h") };
	UFile{ GeneratedPath / fileName }.WriteString(generatedCode);
}

void SGenerator::GenerateSource(UReflectionResult const& reflectionResult)
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
	
	auto const registerFunction{ std::format(R"(
void Register_{0}() 
{{
	SObjectFactory::Register("{0}", +[]() static -> UPtr<KObject> {{ return UCreate<{0}>{{}}(); }});
}}
)",
		classInfo.name
	) };

	const std::string generatedCode{ !classInfo.base.has_value()
		? std::format(R"(
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
	return Cast<{0}>(ptr_);
}}

{4}
)",
			classInfo.name,
			serializeCode.str(),
			deserializeCode.str(),
			memberVariablesCode.str(),
			classInfo.isAbstract ? "" : registerFunction
		)
		: std::format(R"(
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
	return Cast<{0}>(ptr_);
}}

{4}
)",
			classInfo.name,
			serializeCode.str(),
			deserializeCode.str(),
			memberVariablesCode.str(),
			classInfo.isAbstract ? "" : registerFunction
		)
	};

	const UPath fileName{ reflectionResult.path.ToPath().filename().replace_extension(".generated.inl") };
	UFile{ GeneratedPath / fileName }.WriteString(generatedCode);
}

SGenerator::ClassInfo SGenerator::GetClassInfo(UReflectionResult const& reflectionResult)
{
	std::vector<ClassInfo::VariableInfo> variables;
	std::ranges::copy(
		reflectionResult.members
		| std::views::transform([](UReflectionResult::MemberInfo const& member) { return ClassInfo::VariableInfo{ member.type, member.name }; })
		, std::back_inserter(variables)
	);

	return {
		.name = reflectionResult.type.name,
		.base = reflectionResult.type.base,
		.isAbstract = reflectionResult.type.isAbstract,
		.variables = variables,
	};
}
