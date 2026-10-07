#include "Serializer/Serializer.h"

#include "File/File.h"
#include <Logging/log.h>
#include <Path/Path.h>
#include <nlohmann/json.hpp> 

#define KT_LOG_IMPORTANCE_LEVEL_SERIALIZER ELogImportanceLevel::High

void SSerializer::Serialize(nlohmann::json const& fromJson, UPath const& toPath)
{
	if (toPath.IsEmpty())
	{
		KT_LOG(KT_LOG_IMPORTANCE_LEVEL_SERIALIZER, "IO", "can't write data to empty path");
		return;
	}

	if (fromJson.is_null())
	{
		KT_LOG(KT_LOG_IMPORTANCE_LEVEL_SERIALIZER, "IO", "can't write null json to {0}", toPath.ToString());
		return;
	}

	const UFile file{ toPath };
	const std::string jsonString{ fromJson.dump(4) };

	file.WriteString(jsonString);
}

void SSerializer::Deserialize(nlohmann::json& toJson, UPath const& fromPath)
{
	if (fromPath.IsEmpty())
	{
		KT_LOG(KT_LOG_IMPORTANCE_LEVEL_SERIALIZER, "IO", "can't read data from empty path");
		return;
	}

	const UFile file{ fromPath };
	if (!file.Exists())
	{
		KT_LOG(KT_LOG_IMPORTANCE_LEVEL_SERIALIZER, "IO", "file at path {0} doesn't exist", fromPath.ToString());
		return;
	}

	std::istringstream stream{ file.ReadString() };
	stream >> toJson;
}
