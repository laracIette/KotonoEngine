#pragma once
#include <kotono_common/types.h>
#include <filesystem>
#include <span>
class KtFile final
{
public:
	explicit KtFile(const std::filesystem::path& path);

	// Get whether the file exists
	bool Exists() const;
	// Get the path to the file
	const std::filesystem::path& Path() const;
	// Get the path to the directory of the file
	std::filesystem::path GetDirectory() const;
	// Get the name of the file with extension
	std::string GetName() const;
	// Get the extension of the file
	std::string GetExtension() const;
	// Get the name of the file without extension
	std::string GetStem() const;
	// Get the content of the file, for text files
	std::string ReadString() const;
	// Get the content of the file, for binary files
	std::vector<u8> ReadBinary() const;
	// Set the content of the file, for text files
	void WriteString(const std::string_view data) const;
	// Set the content of the file, for binary files
	void WriteBinary(const std::span<u32> data) const;

private:
	const std::filesystem::path path_;
};

