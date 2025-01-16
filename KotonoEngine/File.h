#pragma once
#include <filesystem>
//#define _SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING
//#include <rapidjson/document.h>
//#include <rapidjson/writer.h>
//#include <rapidjson/stringbuffer.h>
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
	// Get the type of the File.
	const std::type_info& GetType() const;

	const std::filesystem::path GetAssetPath() const;

	template <class T>
	const bool IsOfType() const;

	template <class T>
	T* Instantiate() const;

private:
	const std::filesystem::path _path;

	//rapidjson::Document _document;
};
