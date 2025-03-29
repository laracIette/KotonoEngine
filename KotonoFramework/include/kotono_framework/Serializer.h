#pragma once
#include <nlohmann/json_fwd.hpp>
class KtSerializer
{
public:
	nlohmann::json ReadData(const std::filesystem::path& path) const;
	void WriteData(const std::filesystem::path& path, const nlohmann::json& data) const;
};

