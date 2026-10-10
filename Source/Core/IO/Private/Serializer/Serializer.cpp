#include "Serializer/Serializer.h"

#include "File/File.h"
#include <Logging/log.h>
#include <Path/Path.h>
#include <nlohmann/json.hpp> 

auto SSerializer::Serialize(nlohmann::json const& fromJson, UPath const& toPath) -> b8
{
	if (toPath.IsEmpty())
	{
		Log(Normal, "IO", "can't write data to empty path");
		return false;
	}

	if (fromJson.is_null())
	{
		Log(Normal, "IO", "can't write null json to {0}", toPath.ToString());
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
		Log(Normal, "IO", "can't read data from empty path");
		return false;
	}

	UFile const file{ fromPath };
	if (!file.Exists())
	{
		Log(Normal, "IO", "file at path {0} doesn't exist", fromPath.ToString());
		return false;
	}

	std::istringstream stream{ file.ReadString() };
	stream >> toJson;
	
	return true;
}
