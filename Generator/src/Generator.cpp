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
	std::ranges::transform(s, s.begin(),
		[](unsigned char c) { return std::toupper(c); }
	);
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
			"#define GENERATED_{}() \\\n"
			"\tprivate: \\\n"
			"\t\tusing Self = {}; \\\n"
			"\tpublic: \\\n"
			"\t\tvirtual void SerializeTo(nlohmann::json& json) const; \\\n"
			"\t\tvirtual void DeserializeFrom(const nlohmann::json& json); \\\n"
			"\tprivate: \\\n"
			"\t\tUPtr<{}> Ptr() const;\n",
			to_upper(classInfo.name),
			classInfo.name,
			classInfo.name
		)
		: std::format(
			"#define GENERATED_{}() \\\n"
			"\tprivate: \\\n"
			"\t\tusing Self = {}; \\\n"
			"\t\tusing Base = {}; \\\n"
			"\t\tusing Base::Base; \\\n"
			"\tpublic: \\\n"
			"\t\tvoid SerializeTo(nlohmann::json& json) const override; \\\n"
			"\t\tvoid DeserializeFrom(const nlohmann::json& json) override; \\\n"
			"\tprivate: \\\n"
			"\t\tUPtr<{}> Ptr() const;\n",
			to_upper(classInfo.name),
			classInfo.name,
			classInfo.base.value(),
			classInfo.name
		)
	};

	const auto fileHeader{ CorePath / "include" / "kotono_core" / "generated" / reflectionResult.path.ToPath().filename().replace_extension(".generated.h")};
	UFile(fileHeader).WriteString(generatedCodeHeader);


	std::ostringstream serializeCode;
	for (const auto& variable : classInfo.variables)
	{
		serializeCode << "\tserialize(json[\"" << variable << "\"], " << variable << ");\n";
	}
	std::ostringstream deserializeCode;
	for (const auto& variable : classInfo.variables)
	{
		deserializeCode << "\tdeserialize(json.at(\"" << variable << "\"), " << variable << ");\n";
	}

	std::ostringstream objectClassHeaders;
	for (const auto& header : classInfo.headers)
	{
		objectClassHeaders << "#include \"" << header << "\"\n";
	}

	const std::string generatedCodeCPP{ !classInfo.base.has_value()
		? std::format(
			"#include \"{}\"\n"
			"#include \"Ptr.h\"\n"
			"#include \"serialize.h\"\n"
			"#include <nlohmann/json.hpp>\n"
			"{}"
			"\n"
			"void {}::SerializeTo(nlohmann::json& json) const\n"
			"{{\n"
			"{}"
			"}}\n"
			"\n"
			"void {}::DeserializeFrom(const nlohmann::json& json)\n"
			"{{\n"
			"{}"
			"}}\n"
			"\n"
			"UPtr<{}> {}::Ptr() const\n"
			"{{\n"
			"\treturn static_cast<UPtrOwner<{}>*>(ptrOwner_);\n"
			"}}\n",
			reflectionResult.path.ToPath().filename().string(),
			objectClassHeaders.str(),
			classInfo.name,
			serializeCode.str(),
			classInfo.name,
			deserializeCode.str(),
			classInfo.name,
			classInfo.name,
			classInfo.name
		)
		: std::format(
			"#include \"{}\"\n"
			"#include \"Ptr.h\"\n"
			"#include \"serialize.h\"\n"
			"#include <nlohmann/json.hpp>\n"
			"{}"
			"\n"
			"void {}::SerializeTo(nlohmann::json& json) const\n"
			"{{\n"
			"\tBase::SerializeTo(json);\n"
			"{}"
			"}}\n"
			"\n"
			"void {}::DeserializeFrom(const nlohmann::json& json)\n"
			"{{\n"
			"\tBase::DeserializeFrom(json);\n"
			"{}"
			"}}\n"
			"\n"
			"UPtr<{}> {}::Ptr() const\n"
			"{{\n"
			"\treturn static_cast<UPtrOwner<{}>*>(ptrOwner_);\n"
			"}}\n",
			reflectionResult.path.ToPath().filename().string(),
			objectClassHeaders.str(),
			classInfo.name,
			serializeCode.str(),
			classInfo.name,
			deserializeCode.str(),
			classInfo.name,
			classInfo.name,
			classInfo.name
		)
	};

	const auto fileCPP{ CorePath / "src" / "generated" / reflectionResult.path.ToPath().filename().replace_extension(".generated.cpp") };
	UFile(fileCPP).WriteString(generatedCodeCPP);

	std::cout << "Generated " << reflectionResult.path.ToString() << std::endl;
}

SGenerator::ClassInfo SGenerator::GetClassInfo(const UReflectionResult& reflectionResult) const
{
	std::vector<std::string> variables;
	std::ranges::copy(
		reflectionResult.members
		| std::views::transform(&UReflectionResult::MemberInfo::name),
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
