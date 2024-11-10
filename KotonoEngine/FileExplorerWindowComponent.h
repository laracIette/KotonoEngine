#pragma once
#include "FileExplorer.h"
#include <functional>
class FileExplorerWindowComponent final
{
public:
	FileExplorerWindowComponent(FileExplorer* fileExplorer);

	void SetOpenFile(const std::function<void(const std::string&)>& openFile);

private:
	FileExplorer* _fileExplorer;

	std::function<void(const std::string&)> _openFile;
	
	void OnOpenFile(const std::string& filePath) const;
};

