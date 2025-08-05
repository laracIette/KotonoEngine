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

	void SetDirectoryPath(const std::filesystem::path& directoryPath);

	// Get the directories paths in the current directory.
	const std::vector<std::filesystem::path> GetDirectories() const;
	// Get the files whose name contains name.
	const std::vector<KtFile> Find(const std::string& name) const;

	const std::vector<KtFile> GetFiles(const ConditionnalFunction& condition) const;

private:
	std::filesystem::path _directoryPath;
};

