#pragma once
#include <nlohmann/json_fwd.hpp>
class UPath;
class SSerializer final
{
public:
	static void Serialize(nlohmann::json const& json, UPath const& path);
	static void Deserialize(nlohmann::json& json, UPath const& path);
};

