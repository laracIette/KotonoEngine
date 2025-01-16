#pragma once
#include "FileExplorer.h"
#include "EngineInterfaceComponent.h"
#include <functional>
class EICFileExplorer final :
	public EngineInterfaceComponent
{
	BASECLASS(EngineInterfaceComponent)

private:
	typedef std::function<void(const std::string&)> OpenEntryFunction;

	struct Entry
	{
		std::filesystem::path Path;
		OpenEntryFunction OpenEntry;
	};

public:
	EICFileExplorer();

	void Draw() const override;

	void OnResize() override;

private:
	FileExplorer _fileExplorer;

	std::vector<Entry> _directories;
	std::vector<Entry> _files;
};

