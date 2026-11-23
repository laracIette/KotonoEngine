#pragma once

#include <Path/Path.h>
#include <vector>

class UFile;

class UFileExplorer final
{
public:
	UFileExplorer();
	explicit UFileExplorer(UPath&& path);
	explicit UFileExplorer(UPath const& path);

	auto GetDirectoryPath() const -> UPath const&;
	void SetDirectoryPath(UPath const& directoryPath);

	// Get the directories paths in the current directory.
	auto GetDirectories() const -> std::vector<UPath>;

	// Get the files whose name contains name.
	auto Find(const std::string& name) const -> std::vector<UFile>;

	// Get all the files in the directory.
	auto GetFiles(b8 isRecursive = false) const -> std::vector<UFile>;

private:
	UPath directoryPath_;
};

