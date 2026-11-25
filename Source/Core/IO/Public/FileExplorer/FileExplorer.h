#pragma once

#include "Path/Path.h"
#include "Containers/List.h"

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
	auto GetDirectories() const -> UList<UPath>;

	// Get the files whose name contains name.
	auto Find(const std::string& name) const -> UList<UFile>;

	// Get all the files in the directory.
	auto GetFiles(b8 isRecursive = false) const -> UList<UFile>;

private:
	UPath directoryPath_;
};

