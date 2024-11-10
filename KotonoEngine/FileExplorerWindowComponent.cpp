#include "FileExplorerWindowComponent.h"

FileExplorerWindowComponent::FileExplorerWindowComponent(FileExplorer* fileExplorer) :
	_fileExplorer(fileExplorer),
	_openFile(nullptr)
{
}

void FileExplorerWindowComponent::SetOpenFile(const std::function<void(const std::string&)>& openFile)
{
	_openFile = openFile;
}

void FileExplorerWindowComponent::OnOpenFile(const std::string& filePath) const
{
	if (_openFile)
	{
		_openFile(filePath);
	}
}

