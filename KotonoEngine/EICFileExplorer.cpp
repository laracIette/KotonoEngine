#include "EICFileExplorer.h"
#include "KotonoEngine.h"

EICFileExplorer::EICFileExplorer() :
	Base("File Explorer")
{
	std::vector<std::filesystem::path> directories = _fileExplorer.GetDirectories();
	std::transform(
		std::begin(directories), std::end(directories),
		std::back_inserter(_directories),
		[](const std::filesystem::path& path) { return Entry{ path, nullptr }; }
	);

	std::vector<File> files = _fileExplorer.GetFilesOfType<Object>();
	std::transform(
		std::begin(files), std::end(files),
		std::back_inserter(_files),
		[](const File& file) { return Entry{ file.GetPath(), nullptr}; }
	);
}

void EICFileExplorer::Draw() const
{
}

void EICFileExplorer::OnResize()
{
}

