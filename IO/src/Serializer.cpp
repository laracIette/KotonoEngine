#include <nlohmann/json.hpp> 
#include "Serializer.h"
#include "File.h"
#include <fstream>
#include <iostream>
#include <kotono_common/log.h>

#define KT_LOG_IMPORTANCE_LEVEL_SERIALIZER ELogImportanceLevel::High

void USerializer::Serialize(const nlohmann::json& json, const std::filesystem::path& path)
{
	if (path.empty())
	{
		KT_LOG(KT_LOG_IMPORTANCE_LEVEL_SERIALIZER, "IO", "can't write data to empty path");
		return;
	}

	if (json.is_null())
	{
		KT_LOG(KT_LOG_IMPORTANCE_LEVEL_SERIALIZER, "IO", "can't write null json to {}", path.string());
		return;
	}

	const UFile file(path);
	const std::string jsonString{ json.dump(4) };

	file.WriteString(jsonString);
}

void USerializer::Deserialize(nlohmann::json& json, const std::filesystem::path& path)
{
	if (path.empty())
	{
		KT_LOG(KT_LOG_IMPORTANCE_LEVEL_SERIALIZER, "IO", "can't read data from empty path");
		return;
	}

	const UFile file(path);
	if (!file.Exists())
	{
		KT_LOG(KT_LOG_IMPORTANCE_LEVEL_SERIALIZER, "IO", "file at path {} doesn't exist", path.string());
		return;
	}

	std::istringstream stream(file.ReadString());
	stream >> json;
}
