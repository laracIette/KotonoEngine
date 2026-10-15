#include "Generator.h"

#include <File/File.h>
#include <Logging/log.h>
#include <nlohmann/json.hpp>
#include <Path/Path.h>
#include <ranges>
#include <Reflector/Reflector.h>
#include <Serializer/Serializer.h>

static const UPath GENERATED_REGISTRY{ "${ENGINE_DIRECTORY}/Cache/Generator/generated.ktregistry" };
static const UPath GENERATED_DIRECTORY{ "${ENGINE_DIRECTORY}/Cache/Generator/Generated" };

static constexpr auto REGISTRATOR_SOURCE{ R"(
{0}

{1}

void RegisterObjectClasses()
{{
{2}
}}

void UnregisterObjectClasses()
{{
{3}
}}
)" };

static constexpr auto REGISTRY_FUNCTIONS{ R"(
void Register_{0}() 
{{
	SObjectFactory::Register("{0}", +[]() static -> UObjectPtr<KObject> {{ return UCreate<{0}>{{}}(); }});
	{0}::class_ = UPtr{{ new {0}::Class{{}} }};
}}

void Unregister_{0}() 
{{
	delete {0}::class_.Get();
}}
)" };

static constexpr auto CLASS_SOURCE{ R"(
UPtr<{0}::Class const> {0}::class_{{ nullptr }};

auto {0}::Class::Get() -> UPtr<Class const> const&
{{
	return {0}::class_;
}}

auto {0}::Class::Instantiate() const -> UObjectPtr<KObjectReflectionBase>
{{
	return UCreate<{0}>{{}}();
}}

auto {0}::GetClass() const -> UPtr<KObjectReflectionBase::Class const>
{{
	return class_;
}}
)" };

static constexpr auto GENERATED_HEADER_NORMAL{ R"(
#pragma once

#ifdef GENERATED
#undef GENERATED
#endif

#define GENERATED() \
	private: \
		friend void Register_{0}(); \
		friend void Unregister_{0}(); \
	private: \
		using Self = {0}; \
		using Base = {1}; \
		using Base::Base; \
	public: \
		void SerializeTo(nlohmann::json& json) const override; \
		void DeserializeFrom(nlohmann::json const& json) override; \
		void PopulateMemberVariables(std::vector<UVariableInfo>& variables) const override; \
		auto Ptr() const -> UObjectPtr<{0}>; \
		class Class : public Base::Class \
		{{ \
		public: \
			static auto Get() -> UPtr<Class const> const&; \
			auto Instantiate() const -> UObjectPtr<KObjectReflectionBase> override; \
		}}; \
		auto GetClass() const -> UPtr<KObjectReflectionBase::Class const> override; \
	private: \
		static UPtr<Class const> class_; \
	private:
)" };

static constexpr auto GENERATED_HEADER_ABSTRACT{ R"(
#pragma once

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
		void DeserializeFrom(nlohmann::json const& json) override; \
		void PopulateMemberVariables(std::vector<UVariableInfo>& variables) const override; \
		auto Ptr() const -> UObjectPtr<{0}>; \
		class Class : public Base::Class \
		{{ \
		public: \
			auto Instantiate() const -> UObjectPtr<KObjectReflectionBase> override = 0; \
		}}; \
		auto GetClass() const -> UPtr<KObjectReflectionBase::Class const> override = 0; \
	private:
)" };

static constexpr auto GENERATED_SOURCE{ R"(
void {0}::SerializeTo(nlohmann::json& json) const
{{
	Base::SerializeTo(json);
{1}
}}

void {0}::DeserializeFrom(nlohmann::json const& json)
{{
	Base::DeserializeFrom(json);
{2}
}}

void {0}::PopulateMemberVariables(std::vector<UVariableInfo>& variables) const
{{
	Base::PopulateMemberVariables(variables);
	variables.insert(variables.end(), {{
{3}
	}});
}}

auto {0}::Ptr() const -> UObjectPtr<{0}>
{{
	return Cast<{0}>(Pointer);
}}

{4}

{5}
)" };

void SGenerator::GenerateAll()
{
	Log(Normal, "Clearing registry...");

	SSerializer::Serialize(nlohmann::json::object(), GENERATED_REGISTRY);
	GenerateUpdated();
}

void SGenerator::GenerateUpdated()
{
	Log(Normal, "Generating...");

	nlohmann::json json{};
	SSerializer::Deserialize(json, GENERATED_REGISTRY);

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

	SSerializer::Serialize(json, GENERATED_REGISTRY);
}

void SGenerator::GenerateRegistrator()
{
	std::ostringstream registerFunctionsCode;
	std::ostringstream unregisterFunctionsCode;
	std::ostringstream registerCallCode;
	std::ostringstream unregisterCallCode;

	for (auto const& reflectionResult : Reflector.GetReflectionResults())
	{
		if (!reflectionResult.type.isAbstract)
		{
			registerFunctionsCode	<< std::format("extern void Register_{0}();", reflectionResult.type.name) << std::endl;
			unregisterFunctionsCode << std::format("extern void Unregister_{0}();", reflectionResult.type.name) << std::endl;
			registerCallCode		<< std::format("	Register_{0}();", reflectionResult.type.name) << std::endl;
			unregisterCallCode		<< std::format("	Unregister_{0}();", reflectionResult.type.name) << std::endl;
		}
	}

	const std::string generatedCode{ 
		std::format(REGISTRATOR_SOURCE
			, registerFunctionsCode.str()
			, unregisterFunctionsCode.str()
			, registerCallCode.str()
			, unregisterCallCode.str()
		)
	};

	UPath const filePath{ GENERATED_DIRECTORY / "ClassRegistrator.generated.inl" };
	UFile{ filePath }.WriteString(generatedCode);

	Log(Normal, "Generated class registrator");
}

void SGenerator::Generate(UReflectionResult const& reflectionResult)
{
	GenerateHeader(reflectionResult);
	GenerateSource(reflectionResult);

	Log(Normal, "Generated {0}", reflectionResult.path.ToPath().string());
}

void SGenerator::GenerateHeader(UReflectionResult const& reflectionResult)
{
	auto const classInfo{ GetClassInfo(reflectionResult) };
	
	std::string const generatedCode{ classInfo.isAbstract
		? std::format(GENERATED_HEADER_ABSTRACT
			, classInfo.name
			, classInfo.base
		)
		: std::format(GENERATED_HEADER_NORMAL
			, classInfo.name
			, classInfo.base
		)
	};

	UPath const fileName{ reflectionResult.path.ToPath().filename().replace_extension(".generated.h") };
	UFile{ GENERATED_DIRECTORY / fileName }.WriteString(generatedCode);
}

void SGenerator::GenerateSource(UReflectionResult const& reflectionResult)
{
	auto const classInfo{ GetClassInfo(reflectionResult) };

	std::ostringstream serializeCode;
	for (auto const& [type, name] : classInfo.variables)
	{
		serializeCode << std::format(R"(	USerialize<decltype({0})>{{}}(get(json, "{0}"), {0});)", name) << std::endl;
	}

	std::ostringstream deserializeCode;
	for (auto const& [type, name] : classInfo.variables)
	{
		deserializeCode << std::format(R"(	if (contains(json, "{0}")) UDeserialize<decltype({0})>{{}}(get(json, "{0}"), {0});)", name) << std::endl;
	}

	std::ostringstream memberVariablesCode;
	for (auto const& variable : classInfo.variables)
	{
		memberVariablesCode << std::format(R"(		{{ "{0}", "{1}", offsetof(Self, {1}) }},)", variable.type, variable.name) << std::endl;
	}
	
	std::string const instantiateFunction{ std::format(CLASS_SOURCE
		, classInfo.name
	) };
	
	auto const registerFunction{ std::format(REGISTRY_FUNCTIONS
		, classInfo.name
	) };

	const std::string generatedCode{ std::format(GENERATED_SOURCE
		, classInfo.name
		, serializeCode.str()
		, deserializeCode.str()
		, memberVariablesCode.str()
		, classInfo.isAbstract ? "" : instantiateFunction
		, classInfo.isAbstract ? "" : registerFunction
	) };

	const UPath fileName{ reflectionResult.path.ToPath().filename().replace_extension(".generated.inl") };
	UFile{ GENERATED_DIRECTORY / fileName }.WriteString(generatedCode);
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
