#include "Generator.h"

#include <File/File.h>
#include <Logging/log.h>
#include <nlohmann/json.hpp>
#include <Path/Path.h>
#include <ranges>
#include <Reflector/Reflector.h>
#include <Serializer/Serializer.h>

static const UPath RegistryPath{ "${ENGINE_DIRECTORY}/Cache/Generator/generated.ktregistry" };
static const UPath GeneratedPath{ "${ENGINE_DIRECTORY}/Cache/Generator/Generated" };

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
	SObjectFactory::Register("{0}", +[]() static -> UPtr<KObject> {{ return UCreate<{0}>{{}}(); }});
	{0}::Class::instance_ = UPtr{{ new {0}::Class{{}} }};
}}

void Unregister_{0}() 
{{
	delete {0}::Class::instance_.Get();
}}
)" };

static constexpr auto CLASS_HEADER_DERIVED{ R"(\
		private: \
			friend void Register_{0}(); \
			friend void Unregister_{0}(); \
		public: \
			static auto Get() -> UPtr<Class const> const&; \
			auto Instantiate() const -> ObjectPtr override; \
		private: \
			static UPtr<Class const> instance_; \
)" };

static constexpr auto CLASS_SOURCE_DERIVED{ R"(
UPtr<{0}::Class const> {0}::Class::instance_{{ nullptr }};

auto {0}::Class::Get() -> UPtr<Class const> const&
{{
	return instance_;
}}

auto {0}::Class::Instantiate() const -> ObjectPtr
{{
	return UCreate<{0}>{{}}();
}}
)" };

static constexpr auto GENERATED_HEADER_OBJECT{ R"(
#pragma once

#ifdef GENERATED
#undef GENERATED
#endif

#define GENERATED() \
	private: \
		using Self = {0}; \
	public: \
		virtual void SerializeTo(nlohmann::json& json) const; \
		virtual void DeserializeFrom(nlohmann::json const& json); \
		virtual auto GetMemberVariables() const -> std::vector<UVariableInfo>; \
		auto Ptr() const -> UPtr<{0}>; \
		class Class \
		{{ \
		public: \
			virtual ~Class() = default; \
			virtual auto Instantiate() const -> ObjectPtr = 0; \
		}}; \
	private:
)" };

static constexpr auto GENERATED_HEADER_DERIVED{ R"(
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
		auto GetMemberVariables() const -> std::vector<UVariableInfo> override; \
		auto Ptr() const -> UPtr<{0}>; \
		class Class : public Base::Class \
		{{ \
{2} \
		}}; \
	private:
)" };

static constexpr auto GENERATED_SOURCE_OBJECT{ R"(
void {0}::SerializeTo(nlohmann::json& json) const
{{
{1}
}}

void {0}::DeserializeFrom(nlohmann::json const& json)
{{
{2}
}}

auto {0}::GetMemberVariables() const -> std::vector<UVariableInfo>
{{
	return {{
{3}
	}};
}}

auto {0}::Ptr() const -> UPtr<{0}>
{{
	return ptr_;
}}

{4}
)" };

static constexpr auto GENERATED_SOURCE_DERIVED{ R"(
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

auto {0}::GetMemberVariables() const -> std::vector<UVariableInfo>
{{
	auto result{{ Base::GetMemberVariables() }};
	result.insert(result.end(), {{
{3}
	}});
	return result;
}}

auto {0}::Ptr() const -> UPtr<{0}>
{{
	return Cast<{0}>(ptr_);
}}

{4}

{5}
)" };

void SGenerator::GenerateAll()
{
	Log(Normal, "Clearing registry...");

	SSerializer::Serialize(nlohmann::json::object(), RegistryPath);
	GenerateUpdated();
}

void SGenerator::GenerateUpdated()
{
	Log(Normal, "Generating...");

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

	UPath const filePath{ GeneratedPath / "ClassRegistrator.generated.inl" };
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
	
	std::string const instantiateFunction{ std::format(CLASS_HEADER_DERIVED
		, classInfo.name
	) };

	std::string const generatedCode{ !classInfo.base.has_value()
		? std::format(GENERATED_HEADER_OBJECT
			, classInfo.name
		)
		: std::format(GENERATED_HEADER_DERIVED
			, classInfo.name
			, classInfo.base.value()
			, classInfo.isAbstract ? "" : instantiateFunction
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
	
	std::string const instantiateFunction{ std::format(CLASS_SOURCE_DERIVED
		, classInfo.name
	) };
	
	auto const registerFunction{ std::format(REGISTRY_FUNCTIONS
		, classInfo.name
	) };

	const std::string generatedCode{ !classInfo.base.has_value()
		? std::format(GENERATED_SOURCE_OBJECT
			, classInfo.name
			, serializeCode.str()
			, deserializeCode.str()
			, memberVariablesCode.str()
			, classInfo.isAbstract ? "" : registerFunction
		)
		: std::format(GENERATED_SOURCE_DERIVED
			, classInfo.name
			, serializeCode.str()
			, deserializeCode.str()
			, memberVariablesCode.str()
			, classInfo.isAbstract ? "" : instantiateFunction
			, classInfo.isAbstract ? "" : registerFunction
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
