#pragma once
#include <filesystem>
#include <string>
class Generator final
{
	struct ClassInfo
	{
		std::string name;
		std::string baseName;
		std::vector<std::string> variables;
	};

public:
	void Generate() const;

private:
	std::vector<std::filesystem::path> GetHeaders() const;

	ClassInfo GetClassInfo(const std::string& content) const;
	std::string GetClassName(const std::string& content) const;
	std::string GetBaseClassName(const std::string& content) const;
	std::vector<std::string> GetClassVariables(const std::string& content) const;

	bool IsObjectClass(const std::string& className) const;
};