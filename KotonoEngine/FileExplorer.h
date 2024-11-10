#pragma once
#include <vector>
#include "File.h"
class FileExplorer final
{
public:
	FileExplorer();

	void SetDirectoryPath(const std::string& directoryPath);

	// Get the directories paths in the current directory.
	const std::vector<std::string> GetDirectories() const;
	// Get the files paths in the current directory.
	const std::vector<File> GetFiles() const;
	// Get the files whose names contain name.
	const std::vector<File> Find(const std::string& name) const;

private:
	std::string _directoryPath;

	const std::filesystem::directory_iterator GetDirectoryOperator() const;
};

