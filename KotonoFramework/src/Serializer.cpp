#include <nlohmann/json.hpp> // because fwd in Serializer.h
#include "Serializer.h"
#include "File.h"
#include <fstream>
#include <iostream>

void KtSerializer::ReadData(const std::filesystem::path& path, nlohmann::json& json) const
{
	if (path.empty())
	{
		std::cerr << "Can't read data from empty path" << std::endl;
		return;
	}

	const KtFile file(path);
	std::istringstream stream(file.ReadString());
	stream >> json;
}

void KtSerializer::WriteData(const std::filesystem::path& path, const nlohmann::json& json) const
{
	if (path.empty())
	{
		std::cerr << "Can't write data to empty path" << std::endl;
		return;
	}

	if (json.is_null())
	{
		std::cerr << "Error: Cannot write null json to " << path << std::endl;
		return;
	}

	const KtFile file(path);
	const std::string jsonString = json.dump(4);

	file.WriteString(jsonString); 
}