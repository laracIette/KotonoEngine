#pragma once
#include <nlohmann/json_fwd.hpp>
class KtSerializer final
{
public:
	void ReadData(const std::filesystem::path& path, nlohmann::json& json) const;
	void WriteData(const std::filesystem::path& path, const nlohmann::json& json) const;
};

