#pragma once
#include <types.h>
#include <nlohmann/json_fwd.hpp>

class UPath;

class SSerializer final
{
public:
	static auto Serialize(nlohmann::json const& fromJson, UPath const& toPath) -> b8;
	static auto Deserialize(nlohmann::json& toJson, UPath const& fromPath) -> b8;
};

