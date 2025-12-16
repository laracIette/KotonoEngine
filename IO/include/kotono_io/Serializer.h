#pragma once
#include <nlohmann/json_fwd.hpp>
#include <filesystem>
class KtSerializer final
{
public:
	static void Serialize(const nlohmann::json& json, const std::filesystem::path& path);
	static void Deserialize(nlohmann::json& json, const std::filesystem::path& path);
};

