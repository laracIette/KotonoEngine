#include "Serializer/Serializer.h"

#include "File/File.h"
#include <Logging/log.h>
#include <Path/Path.h>
#include <nlohmann/json.hpp> 

#define KT_LOG_IMPORTANCE_LEVEL_SERIALIZER ELogImportanceLevel::High

auto SSerializer::Serialize(nlohmann::json const& fromJson, UPath const& toPath) -> b8
{
	if (toPath.IsEmpty())
	{
		KT_LOG(KT_LOG_IMPORTANCE_LEVEL_SERIALIZER, "IO", "can't write data to empty path");
		return false;
	}

	if (fromJson.is_null())
	{
		KT_LOG(KT_LOG_IMPORTANCE_LEVEL_SERIALIZER, "IO", "can't write null json to {0}", toPath.ToString());
		return false;
	}

	std::string const jsonString{ fromJson.dump(4) };
	UFile{ toPath }.WriteString(jsonString);
	
	return true;
}

auto SSerializer::Deserialize(nlohmann::json& toJson, UPath const& fromPath) -> b8
{
	if (fromPath.IsEmpty())
	{
		KT_LOG(KT_LOG_IMPORTANCE_LEVEL_SERIALIZER, "IO", "can't read data from empty path");
		return false;
	}

	UFile const file{ fromPath };
	if (!file.Exists())
	{
		KT_LOG(KT_LOG_IMPORTANCE_LEVEL_SERIALIZER, "IO", "file at path {0} doesn't exist", fromPath.ToString());
		return false;
	}

	std::istringstream stream{ file.ReadString() };
	stream >> toJson;
	
	return true;
}
