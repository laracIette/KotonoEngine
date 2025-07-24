#include <nlohmann/json.hpp> // because fwd in Serializer.h
#include "Serializer.h"
#include "File.h"
#include <fstream>
#include <iostream>
#include "log.h"

#define KT_LOG_IMPORTANCE_LEVEL_SERIALIZER KT_LOG_IMPORTANCE_LEVEL_HIGH

void KtSerializer::ReadData(const std::filesystem::path& path, nlohmann::json& json) const
{
	if (path.empty())
	{
		KT_LOG_KF(KT_LOG_IMPORTANCE_LEVEL_SERIALIZER, "can't read data from empty path");
		return;
	}

	const KtFile file(path);
	if (!file.Exists())
	{
		KT_LOG_KF(KT_LOG_IMPORTANCE_LEVEL_SERIALIZER, "file at path '%s' doesn't exist", path.string().c_str());
		return;
	}

	std::istringstream stream(file.ReadString());
	stream >> json;
}

void KtSerializer::WriteData(const std::filesystem::path& path, const nlohmann::json& json) const
{
	if (path.empty())
	{
		KT_LOG_KF(KT_LOG_IMPORTANCE_LEVEL_SERIALIZER, "can't write data to empty path");
		return;
	}

	if (json.is_null())
	{
		KT_LOG_KF(KT_LOG_IMPORTANCE_LEVEL_SERIALIZER, "can't write null json to %s", path.string().c_str());
		return;
	}

	const KtFile file(path);
	const std::string jsonString = json.dump(4);

	file.WriteString(jsonString); 
}