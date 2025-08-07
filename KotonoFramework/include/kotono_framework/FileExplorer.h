#pragma once
#include "File.h"
#include <vector>
#include <functional>
class KtFileExplorer final
{
private:
	using ConditionnalFunction = std::function<bool(const std::filesystem::path)>;

public:
	KtFileExplorer();

	const std::filesystem::path& GetDirectoryPath() const;
	void SetDirectoryPath(const std::filesystem::path& directoryPath);

	// Get the directories paths in the current directory.
	std::vector<std::filesystem::path> GetDirectories() const;
	// Get the files whose name contains name.
	std::vector<KtFile> Find(const std::string& name) const;
	// Get the files that match the condition.
	std::vector<KtFile> GetFiles(const ConditionnalFunction& condition) const;

private:
	std::filesystem::path directoryPath_;
};

