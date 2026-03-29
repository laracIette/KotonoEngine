#pragma once
#include <nlohmann/json_fwd.hpp>
#include <filesystem>
class UPath;
class USerializer final
{
public:
	static void Serialize(const nlohmann::json& json, const UPath& path);
	static void Deserialize(nlohmann::json& json, const UPath& path);
};

