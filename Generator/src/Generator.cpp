#include "Generator.h"
#include <iostream>
#include <kotono_common/Path.h>
#include <kotono_io/File.h>
#include <kotono_io/Serializer.h>
#include <kotono_reflection/Reflector.h>
#include <nlohmann/json.hpp>
#include <print>
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

void SGenerator::GenerateAll() const
{
	std::println("Clearing registry...");

	USerializer::Serialize(nlohmann::json::object(), RegistryPath);
	GenerateUpdated();
}

void SGenerator::GenerateUpdated() const
{
	std::println("Generating...");

	nlohmann::json json{};
	USerializer::Deserialize(json, RegistryPath);

	const auto& reflectionResults{ Reflector.GetReflectionResults() };
	for (const auto& reflectionResult : reflectionResults)
	{
		const UFile file(reflectionResult.path);

		const auto entryPath{ reflectionResult.path.ToString() };
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
	GenerateHeader(reflectionResult);
	GenerateSource(reflectionResult);

	std::println("Generated {0}", reflectionResult.path.ToPath().string());
}

void SGenerator::GenerateHeader(const UReflectionResult& reflectionResult) const
{
	const auto classInfo{ GetClassInfo(reflectionResult) };

	const std::string generatedCode{ !classInfo.base.has_value()
		? std::format(
R"(#define GENERATED_{0}() \
	private: \
		static UAutoRegister register_; \
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
R"(#define GENERATED_{0}() \
	private: \
		static UAutoRegister register_; \
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

	const UPath fileDirectory{ reflectionResult.path.Directory() };
	const UPath fileName{ reflectionResult.path.ToPath().filename().replace_extension(".generated.h") };
	UFile(fileDirectory / "generated" / fileName).WriteString(generatedCode);
}

void SGenerator::GenerateSource(const UReflectionResult & reflectionResult) const
{
	const auto classInfo{ GetClassInfo(reflectionResult) };

	std::ostringstream serializeCode;
	for (const auto& variable : classInfo.variables)
	{
		serializeCode << std::format(R"(	USerialize<decltype({0})>{{}}(get(json, "{0}"), {0});)", variable.name) << std::endl;
	}

	std::ostringstream deserializeCode;
	for (const auto& variable : classInfo.variables)
	{
		deserializeCode << std::format(R"(	UDeserialize<decltype({0})>{{}}(get(json, "{0}"), {0});)", variable.name) << std::endl;
	}

	std::ostringstream memberVariablesCode;
	for (const auto& variable : classInfo.variables)
	{
		memberVariablesCode << std::format(R"(		{{ "{0}", "{1}", offsetof(Self, {1}) }},)", variable.type, variable.name) << std::endl;
	}

	const std::string generatedCode{ !classInfo.base.has_value()
		? std::format(
R"(UAutoRegister {0}::register_("{0}", []() {{ return Create<{0}>{{}}(); }});

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
	return UPtr<{0}>(ptrOwner_);
}}
)",
			classInfo.name,
			serializeCode.str(),
			deserializeCode.str(),
			memberVariablesCode.str()
		)
		: std::format(
R"(UAutoRegister {0}::register_("{0}", []() {{ return Create<{0}>{{}}(); }});

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
	return UPtr<{0}>(ptrOwner_);
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
	UFile(fileDirectory / "generated" / fileName).WriteString(generatedCode);
}

SGenerator::ClassInfo SGenerator::GetClassInfo(const UReflectionResult& reflectionResult) const
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
