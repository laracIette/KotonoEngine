#pragma once
#include <nlohmann/json_fwd.hpp>
class UPath;
class SSerializer final
{
public:
	static void Serialize(nlohmann::json const& fromJson, UPath const& toPath);
	static void Deserialize(nlohmann::json& toJson, UPath const& fromPath);
};

