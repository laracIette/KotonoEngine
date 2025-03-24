#pragma once
#include <filesystem>
class KtFile final
{
public:
	KtFile(const std::filesystem::path& path);

	// Get the path to the file.
	const std::filesystem::path& GetPath() const;
	// Get the path to the directory of the file.
	const std::filesystem::path GetDirectory() const;
	// Get the name of the file with extension.
	const std::string GetName() const;
	// Get the extension of the file.
	const std::string GetExtension() const;
	// Get the name of the file without extension.
	const std::string GetStem() const;
	// Get the content of the file, for text files.
	const std::string GetTextContent() const;
	// Get the content of the file, for all files.
	const std::vector<uint8_t> GetBinaryContent() const;

private:
	const std::filesystem::path _path;
};

