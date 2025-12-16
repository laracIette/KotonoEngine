#pragma once
#include <kotono_io/Serializer.h>
#include <kotono_common/Path.h>
#include <nlohmann/json.hpp>
class SProjectSettings final
{
public:
	template <typename T>
	static T Get(const std::string& name)
	{
		nlohmann::json json{};
		KtSerializer::Deserialize(json, Path.Project() / "projectSettings.json");

		return json.at(nlohmann::json::json_pointer(name)).get<T>();
	}

	static void Set(const std::string& name, const auto& value)
	{
		nlohmann::json json{};
		KtSerializer::Deserialize(json, Path.Project() / "projectSettings.json");

		json.at(nlohmann::json::json_pointer(name)) = value;
		KtSerializer::Serialize(json, Path.Project() / "projectSettings.json");
	}
};

inline SProjectSettings ProjectSettings;