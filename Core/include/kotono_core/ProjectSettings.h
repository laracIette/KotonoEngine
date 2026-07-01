#pragma once
#include <kotono_common/Path.h>
#include <kotono_io/Serializer.h>
#include <nlohmann/json.hpp>
class SProjectSettings final
{
public:
	static const UPath& Path();

	template <typename T>
	static T Get(const std::string& name)
	{
		nlohmann::json json{};
		SSerializer::Deserialize(json, Path());

		return json.at(nlohmann::json::json_pointer(name)).get<T>();
	}

	static void Set(const std::string& name, const auto& value)
	{
		nlohmann::json json{};
		SSerializer::Deserialize(json, Path());

		json.at(nlohmann::json::json_pointer(name)) = value;
		SSerializer::Serialize(json, Path());
	}
};
