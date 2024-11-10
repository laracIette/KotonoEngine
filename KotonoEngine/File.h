#pragma once
#include <filesystem>
class File final
{
public:
	File(const std::filesystem::path& path);


	// Get the path to the File.
	const std::filesystem::path& GetPath() const;
	// Get the path to the directory of the File.
	const std::filesystem::path GetDirectory() const;
	// Get the name of the File with extension.
	const std::string GetName() const;
	// Get the extension of the File.
	const std::string GetExtension() const;
	// Get the name of the File without extension.
	const std::string GetStem() const;
	// Get the content of the File.
	const std::string GetContent() const;

private:
	const std::filesystem::path _path;
};

