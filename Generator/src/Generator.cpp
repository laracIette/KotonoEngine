#include "Generator.h"
#include <fstream>
#include <regex>
#include <iostream>
#include <nlohmann/json.hpp>

static const auto CorePath{ std::filesystem::path(ENGINE_DIRECTORY) / "Core" };
static const auto RegistryPath{ CorePath / "objects.ktregistry" };

static std::string read_string(const std::filesystem::path& path)
{
	// Check if path exists.
	if (!exists(path))
	{
		printf("Failed to find a file at '%s'\n", path.string().c_str());
		return "";
	}

	// Open file
	std::ifstream file(path);

	if (!file.is_open())
	{
		printf("Failed to open the file at '%s'\n", path.string().c_str());
		return "";
	}

	// Read file
	std::ostringstream fileContents;
	fileContents << file.rdbuf();

	file.close();

	return fileContents.str();
}

static void write_string(const std::filesystem::path& path, const std::string_view data)
{
	// Open file for writing
	std::ofstream file(path, std::ios::out | std::ios::trunc);

	if (!file.is_open())
	{
		printf("Failed to open the file at '%s' for writing.\n", path.string().c_str());
		return;
	}

	// Write data to file
	file << data;

	// Close file
	file.close();
}

static void read_data(const std::filesystem::path& path, nlohmann::json& json)
{
	if (path.empty())
	{
		printf("can't read data from empty path\n");
		return;
	}

	if (!exists(path))
	{
		printf("file at path '%s' doesn't exist\n", path.string().c_str());
		return;
	}

	std::istringstream stream(read_string(path));
	stream >> json;
}

static void write_data(const std::filesystem::path& path, const nlohmann::json& json)
{
	if (path.empty())
	{
		printf("can't write data to empty path\n");
		return;
	}

	if (json.is_null())
	{
		printf("can't write null json to '%s'\n", path.string().c_str());
		return;
	}

	const std::string jsonString{ json.dump(4) };
	write_string(path, jsonString);
}

static std::string to_upper(std::string s)
{
	std::ranges::transform(s, s.begin(),
		[](unsigned char c) { return std::toupper(c); }
	);
	return s;
}

void Generator::GenerateAll() const
{
	std::cout << "Clearing registry..." << std::endl;
	write_data(RegistryPath, nlohmann::json::object());
	GenerateUpdated();
}

void Generator::GenerateUpdated() const
{
	std::cout << "Generating..." << std::endl;

	nlohmann::json json{};
	
	read_data(RegistryPath, json);

	const auto headerDirectoryPath{ CorePath / "include" / "kotono_core" };
	for (const auto& entry : std::filesystem::directory_iterator(headerDirectoryPath))
	{
		if (!entry.is_regular_file() || entry.path().extension() != ".h")
		{
			continue;
		}

		const auto entryPath{ entry.path().filename().string() };

		const auto ftime{ entry.last_write_time() };
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
				Generate(entry.path());
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
			Generate(entry.path());
		}
	}

	write_data(RegistryPath, json);
}

Generator::ClassInfo Generator::GetClassInfo(const std::string& content) const
{
	return {
		.name = GetClassName(content),
		.baseName = GetBaseClassName(content),
		.variables = GetClassVariables(content),
		.fwdClasses = GetForwardDeclaredClasses(content),
	};
}

std::string Generator::GetClassName(const std::string& content) const
{
	const std::regex classRegex(R"(class\s+([A-Za-z_]\w*)\s*(?:\s*:\s*[^{]+)?\s*\{)");
	std::smatch match;

	if (std::regex_search(content, match, classRegex))
	{
		return match[1].str();
	}

	return "";
}

std::string Generator::GetBaseClassName(const std::string& content) const
{
	const std::regex baseRegex(R"(class\s+[A-Za-z_]\w*\s*:\s*(?:public|private|protected)?\s*([A-Za-z_][\w:<>]*)\s*\{?)");
	std::smatch match;

	if (std::regex_search(content, match, baseRegex))
	{
		return match[1].str();
	}

	return "";
}

std::vector<Generator::VariableInfo> Generator::GetClassVariables(const std::string& content) const
{
	std::vector<VariableInfo> result;

	const std::regex varRegex(R"(SERIALIZE\s*\(?\s*\)?\s*(.+?)\s+([A-Za-z_]\w*)\s*;)");

	for (std::sregex_iterator it(content.begin(), content.end(), varRegex), end; it != end; ++it)
	{
		const auto typeName{ (*it)[1].str() };
		const auto varName{ (*it)[2].str() };

		const bool isIterable{
			typeName.find("KtPool") != std::string::npos ||
			typeName.find("std::vector") != std::string::npos ||
			typeName.find("std::array") != std::string::npos
		};

		result.push_back({ varName, isIterable });
	}

	return result;
}

std::vector<std::string> Generator::GetForwardDeclaredClasses(const std::string& content) const
{
	std::vector<std::string> result;

	const std::regex fwdRegex(R"(\bclass\s+(?:\w+\s+)*([A-Za-z_]\w*)\s*;)");

	for (std::sregex_iterator it(content.begin(), content.end(), fwdRegex), end; it != end; ++it)
	{
		result.push_back((*it)[1]);
	}

	return result;
}

std::string Generator::GetObjectClassHeader(const std::string& className) const
{
	return std::format("{}.h", className.substr(1));
}

bool Generator::IsObjectClass(const std::string& className) const
{
	if (className.empty()
	 || className.starts_with("Kt"))
	{
		return false;
	}
	return className[0] == 'K'
		|| className[0] == 'T'
		|| className[0] == 'R';
}

void Generator::Generate(const std::filesystem::path& header) const
{
	const auto content{ read_string(header) };
	const auto classInfo{ GetClassInfo(content) };

	if (!IsObjectClass(classInfo.name))
	{
		return;
	}

	const bool isKObject{ classInfo.name == "KObject" };

	const std::string generatedCodeHeader{ isKObject
		? std::format(
			"#define GENERATED_{}() \\\n"
			"\tpublic: \\\n"
			"\t\tvirtual void SerializeTo(nlohmann::json& json) const; \\\n"
			"\t\tvirtual void DeserializeFrom(const nlohmann::json& json); \\\n"
			"\tprivate: \\\n"
			"\t\tUPtr<{}> Ptr() const;\n",
			to_upper(classInfo.name),
			classInfo.name
		)
		: std::format(
			"#define GENERATED_{}() \\\n"
			"\tprivate: \\\n"
			"\t\tusing Base = {}; \\\n"
			"\t\tusing Base::Base; \\\n"
			"\tpublic: \\\n"
			"\t\tvoid SerializeTo(nlohmann::json& json) const override; \\\n"
			"\t\tvoid DeserializeFrom(const nlohmann::json& json) override; \\\n"
			"\tprivate: \\\n"
			"\t\tUPtr<{}> Ptr() const;\n",
			to_upper(classInfo.name),
			classInfo.baseName,
			classInfo.name
		)
	};

	const auto fileHeader{ CorePath / "include" / "kotono_core" / "generated" / header.filename().replace_extension(".generated.h") };
	write_string(fileHeader, generatedCodeHeader);


	std::ostringstream serializeCode;
	for (const auto& variable : classInfo.variables)
	{
		if (variable.isIterable)
		{
			serializeCode
				<< "\tjson[\"" << variable.name << "\"] = nlohmann::json::array({});\n"
				<< "\tfor (size_t i{ 0 }; i < " << variable.name << ".size(); ++i)\n"
				<< "\t{\n"
				<< "\t\tserialize(json[\"" << variable.name << "\"][i], " << variable.name << "[i]);\n"
				<< "\t}\n";
		}
		else
		{
			serializeCode << "\tserialize(json[\"" << variable.name << "\"], " << variable.name << ");\n";
		}
	}
	std::ostringstream deserializeCode;
	for (const auto& variable : classInfo.variables)
	{
		if (variable.isIterable)
		{
			deserializeCode
				<< "\t" << variable.name << ".resize(json.at(\"" << variable.name << "\").size()); \n"
				<< "\tfor (size_t i{ 0 }; i < " << variable.name << ".size(); ++i)\n"
				<< "\t{\n"
				<< "\t\tdeserialize(json.at(\"" << variable.name << "\")[i], " << variable.name << "[i]);\n"
				<< "\t}\n";
		}
		else
		{
			deserializeCode << "\tdeserialize(json.at(\"" << variable.name << "\"), " << variable.name << ");\n";
		}
	}

	std::ostringstream objectClassHeaders;
	for (const auto& className : classInfo.fwdClasses)
	{
		if (IsObjectClass(className))
		{
			objectClassHeaders << "#include \"" << GetObjectClassHeader(className) << "\"\n";
		}
	}

	const std::string generatedCodeCPP{ isKObject
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
			header.filename().string(),
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
			header.filename().string(),
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

	const auto fileCPP{ CorePath / "src" / "generated" / header.filename().replace_extension(".generated.cpp") };
	write_string(fileCPP, generatedCodeCPP);

	std::cout << "Generated " << header << std::endl;
}
