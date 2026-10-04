#pragma once
#include <kotono_common/Path.h>
#include <vector>
class UFile;
class UFileExplorer final
{
public:
	UFileExplorer();
	UFileExplorer(const UPath& path);

	const UPath& GetDirectoryPath() const;
	void SetDirectoryPath(const UPath& directoryPath);

	// Get the directories paths in the current directory.
	std::vector<UPath> GetDirectories() const;
	// Get the files whose name contains name.
	std::vector<UFile> Find(const std::string& name) const;
	// Get all the files in the directory.
	std::vector<UFile> GetFiles(const bool isRecursive = false) const;

private:
	UPath directoryPath_;
};

