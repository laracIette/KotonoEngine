#pragma once
#include <vector>
#include "File.h"
class FileExplorer final
{
public:
	FileExplorer();

	void SetDirectoryPath(const std::filesystem::path& directoryPath);

	// Get the directories paths in the current directory.
	const std::vector<std::filesystem::path> GetDirectories() const;
	// Get the files of the type type.
	template <class T>
	const std::vector<File> GetFilesOfType() const;
	// Get the files whose names contain name.
	const std::vector<File> Find(const std::string& name) const;

private:
	std::filesystem::path _directoryPath;
};

