#include "FileExplorerComponent.h"

FileExplorerComponent::FileExplorerComponent() :
	_openFile(nullptr)
{
}

void FileExplorerComponent::Init()
{
	SetName("File Explorer");

	// Open project
	_fileExplorer.SetDirectoryPath(R"(C:\Users\nicos\Documents\Kotono Projects\Test)");
}

void FileExplorerComponent::SetOpenFile(const std::function<void(const std::string&)>& openFile)
{
	_openFile = openFile;
}

void FileExplorerComponent::OnOpenFile(const std::string& filePath) const
{
	if (_openFile)
	{
		_openFile(filePath);
	}
}

