#pragma once
#include <kotono_io/Serializer.h>
#include <nlohmann/json.hpp>
class SProjectSettings final
{
public:
	static std::filesystem::path Path();

	template <typename T>
	static T Get(const std::string& name)
	{
		nlohmann::json json{};
		USerializer::Deserialize(json, Path());

		return json.at(nlohmann::json::json_pointer(name)).get<T>();
	}

	static void Set(const std::string& name, const auto& value)
	{
		nlohmann::json json{};
		USerializer::Deserialize(json, Path());

		json.at(nlohmann::json::json_pointer(name)) = value;
		USerializer::Serialize(json, Path());
	}
};
