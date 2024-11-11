#pragma once
#include "FileExplorer.h"
#include "Component.h"
#include <functional>
class FileExplorerComponent final :
	public Component
{
	BASECLASS(Component)

public:
	FileExplorerComponent();

	void Init() override;

	void SetOpenFile(const std::function<void(const std::string&)>& openFile);

private:
	FileExplorer _fileExplorer;

	std::function<void(const std::string&)> _openFile;
	
	void OnOpenFile(const std::string& filePath) const;
};

