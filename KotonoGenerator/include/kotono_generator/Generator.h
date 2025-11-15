#pragma once
#include <filesystem>
#include <string>
class Generator final
{
	struct VariableInfo
	{
		std::string name;
		bool isIterable;
	};

	struct ClassInfo
	{
		std::string name;
		std::string baseName;
		std::vector<VariableInfo> variables;
		std::vector<std::string> fwdClasses;
	};

public:
	void GenerateAll() const;
	void GenerateUpdated() const;

private:
	ClassInfo GetClassInfo(const std::string& content) const;
	std::string GetClassName(const std::string& content) const;
	std::string GetBaseClassName(const std::string& content) const;
	std::vector<VariableInfo> GetClassVariables(const std::string& content) const;
	std::vector<std::string> GetForwardDeclaredClasses(const std::string& content) const;
	std::string GetObjectClassHeader(const std::string& className) const;

	bool IsObjectClass(const std::string& className) const;

	void Generate(const std::filesystem::path& header) const;
};