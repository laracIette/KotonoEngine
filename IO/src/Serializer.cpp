#include "Serializer.h"
#include "File.h"
#include <kotono_common/log.h>
#include <kotono_common/Path.h>
#include <nlohmann/json.hpp> 

#define KT_LOG_IMPORTANCE_LEVEL_SERIALIZER ELogImportanceLevel::High

void SSerializer::Serialize(const nlohmann::json& json, const UPath& path)
{
	if (path.IsEmpty())
	{
		KT_LOG(KT_LOG_IMPORTANCE_LEVEL_SERIALIZER, "IO", "can't write data to empty path");
		return;
	}

	if (json.is_null())
	{
		KT_LOG(KT_LOG_IMPORTANCE_LEVEL_SERIALIZER, "IO", "can't write null json to {}", path.ToString());
		return;
	}

	const UFile file(path);
	const std::string jsonString{ json.dump(4) };

	file.WriteString(jsonString);
}

void SSerializer::Deserialize(nlohmann::json& json, const UPath& path)
{
	if (path.IsEmpty())
	{
		KT_LOG(KT_LOG_IMPORTANCE_LEVEL_SERIALIZER, "IO", "can't read data from empty path");
		return;
	}

	const UFile file(path);
	if (!file.Exists())
	{
		KT_LOG(KT_LOG_IMPORTANCE_LEVEL_SERIALIZER, "IO", "file at path {} doesn't exist", path.ToString());
		return;
	}

	std::istringstream stream(file.ReadString());
	stream >> json;
}
