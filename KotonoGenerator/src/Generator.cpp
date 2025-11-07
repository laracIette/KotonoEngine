#include "Generator.h"
#include <fstream>
#include <regex>

static std::string to_upper(std::string s)
{
	std::ranges::transform(s, s.begin(), 
		[](unsigned char c) { return std::toupper(c); }
	);
	return s;
}

static std::string read_file(const std::filesystem::path& path)
{
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

static void write_file(const std::filesystem::path& path, const std::string_view data)
{
	// Open file for writing
	std::ofstream file(path, std::ios::out | std::ios::trunc);

	if (!file.is_open())
	{
		printf("Failed to open the file at '%s' for writing. \n", path.string().c_str());
		return;
	}

	// Write data to file
	file << data;

	// Close file
	file.close();
}

void Generator::Generate() const
{
	for (const auto& header : GetHeaders())
	{
		const auto content{ read_file(header) };
		const auto classInfo{ GetClassInfo(content) };
		if (!IsObjectClass(classInfo.name))
		{
			continue;
		}

		const bool isKObject{ classInfo.name == "KObject" };

		const std::string generatedCodeHeader{ isKObject
			? std::format(
				"#define GENERATED_{}() \\\n"
				"\tpublic: \\\n"
				"\t\tvirtual void SerializeTo(nlohmann::json& json) const; \\\n"
				"\t\tvirtual void DeserializeFrom(const nlohmann::json& json); \\\n"
				"\tprivate:\n",
				to_upper(classInfo.name)
			)
			: std::format(
				"#define GENERATED_{}() \\\n"
				"\tprivate: \\\n"
				"\t\tusing Base = {}; \\\n"
				"\t\tusing Base::Base; \\\n"
				"\tpublic: \\\n"
				"\t\tvoid SerializeTo(nlohmann::json& json) const override; \\\n"
				"\t\tvoid DeserializeFrom(const nlohmann::json& json) override; \\\n"
				"\tprivate:\n",
				to_upper(classInfo.name),
				classInfo.baseName
			)	
		};

		const auto generatedPathHeader{ header.parent_path() / "generated" / header.filename().replace_extension(".generated.h") };
		write_file(generatedPathHeader, generatedCodeHeader);


		std::ostringstream serializeCode;
		for (const auto& variable : classInfo.variables)
		{
			serializeCode << "\tserialize(json[\"" << variable << "\"], " << variable << ");\n";
		}
		std::ostringstream deserializeCode;
		for (const auto& variable : classInfo.variables)
		{
			deserializeCode << "\tdeserialize(json[\"" << variable << "\"], " << variable << ");\n";
		}

		const std::string generatedCodeCPP{ isKObject
			? std::format(
				"#include \"{}\"\n"
				"#include \"serialize.h\"\n"
				"#include <nlohmann/json.hpp>\n"
				"\n"
				"void {}::SerializeTo(nlohmann::json& json) const\n"
				"{{\n"
				"{}"
				"}}\n"
				"\n"
				"void {}::DeserializeFrom(const nlohmann::json& json)\n"
				"{{\n"
				"{}"
				"}}\n",
				header.filename().string(),
				classInfo.name,
				serializeCode.str(),
				classInfo.name,
				deserializeCode.str()
			)
			: std::format(
				"#include \"{}\"\n"
				"#include \"serialize.h\"\n"
				"#include <nlohmann/json.hpp>\n"
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
				"}}\n",
				header.filename().string(),
				classInfo.name,
				serializeCode.str(),
				classInfo.name,
				deserializeCode.str()
			) 
		};

		const auto generatedPathCPP{ header.parent_path().parent_path().parent_path() / "src/generated" / header.filename().replace_extension(".generated.cpp") };
		write_file(generatedPathCPP, generatedCodeCPP);
	}
}

std::vector<std::filesystem::path> Generator::GetHeaders() const
{
	std::vector<std::filesystem::path> result{};

	const auto headerDirectoryPath{ "W:/Visual Studio/Projects/KotonoEngine/KotonoEngine/include/kotono_engine" };

	for (const auto& entry : std::filesystem::directory_iterator(headerDirectoryPath))
	{
		if (entry.is_regular_file() && entry.path().extension() == ".h")
		{
			result.push_back(entry.path());
		}
	}

	return result;
}

Generator::ClassInfo Generator::GetClassInfo(const std::string& content) const
{
	return {
		.name = GetClassName(content),
		.baseName = GetBaseClassName(content),
		.variables = GetClassVariables(content),
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

std::vector<std::string> Generator::GetClassVariables(const std::string& content) const
{
	std::vector<std::string> result{};

	const std::regex varRegex(R"(SERIALIZE\s*\(?\s*\)?[^\n]*\b([A-Za-z_]\w*)\b)");

	for (std::sregex_iterator i(content.begin(), content.end(), varRegex), end; i != end; ++i)
	{
		result.push_back((*i)[1]);
	}

	return result;
}

bool Generator::IsObjectClass(const std::string& className) const
{
	if (className.empty())
	{
		return false;
	}
	return className[0] == 'K' 
		|| className[0] == 'T'
		|| className[0] == 'R';
}
