#include <nlohmann/json.hpp>
#include "Serializer.h"
#include "File.h"
#include <fstream>
#include <iostream>

nlohmann::json KtSerializer::ReadData(const std::filesystem::path& path) const
{
	nlohmann::json data;

	const KtFile file(path);
	std::istringstream stream(file.ReadString());
	stream >> data;

	return data;
}

void KtSerializer::WriteData(const std::filesystem::path& path, const nlohmann::json& data) const
{
	if (data.is_null())
	{
		std::cerr << "Error: Cannot write null JSON data to " << path << std::endl;
		return;
	}

	const KtFile file(path);
	const std::string jsonString = data.dump(4);

	file.WriteString(jsonString); 
}