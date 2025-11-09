#pragma once
#include <filesystem>
#include <string>
class Generator final
{
	struct VariableInfo
	{
		std::string name;
		bool isContainer;
	};

	struct ClassInfo
	{
		std::string name;
		std::string baseName;
		std::vector<VariableInfo> variables;
		std::vector<std::string> fwdClasses;
	};

public:
	void Generate() const;

private:
	std::vector<std::filesystem::path> GetHeaders() const;

	ClassInfo GetClassInfo(const std::string& content) const;
	std::string GetClassName(const std::string& content) const;
	std::string GetBaseClassName(const std::string& content) const;
	std::vector<VariableInfo> GetClassVariables(const std::string& content) const;
	std::vector<std::string> GetForwardDeclaredClasses(const std::string& content) const;
	std::string GetObjectClassHeader(const std::string& className) const;

	bool IsObjectClass(const std::string& className) const;
};