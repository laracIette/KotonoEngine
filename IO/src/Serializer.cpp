#include <nlohmann/json.hpp> 
#include "Serializer.h"
#include "File.h"
#include <fstream>
#include <iostream>
#include <kotono_common/log.h>

#define KT_LOG_IMPORTANCE_LEVEL_SERIALIZER KT_LOG_IMPORTANCE_LEVEL_HIGH

void KtSerializer::Serialize(const nlohmann::json& json, const std::filesystem::path& path)
{
	if (path.empty())
	{
		KT_LOG(KT_LOG_IMPORTANCE_LEVEL_SERIALIZER, "IO.KtSerializer::Serialize()", "can't write data to empty path");
		return;
	}

	if (json.is_null())
	{
		KT_LOG(KT_LOG_IMPORTANCE_LEVEL_SERIALIZER, "IO.KtSerializer::Serialize()", "can't write null json to %s", path.string().c_str());
		return;
	}

	const KtFile file(path);
	const std::string jsonString{ json.dump(4) };

	file.WriteString(jsonString);
}

void KtSerializer::Deserialize(nlohmann::json& json, const std::filesystem::path& path)
{
	if (path.empty())
	{
		KT_LOG(KT_LOG_IMPORTANCE_LEVEL_SERIALIZER, "IO.KtSerializer::Deserialize()", "can't read data from empty path");
		return;
	}

	const KtFile file(path);
	if (!file.Exists())
	{
		KT_LOG(KT_LOG_IMPORTANCE_LEVEL_SERIALIZER, "IO.KtSerializer::Deserialize()", "file at path %s doesn't exist", path.string().c_str());
		return;
	}

	std::istringstream stream(file.ReadString());
	stream >> json;
}
