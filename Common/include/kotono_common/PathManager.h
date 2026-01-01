#pragma once
#include <filesystem>
class SPathManager final
{
public:
	std::filesystem::path Engine() const;
	const std::filesystem::path& Project() const;

private:
	static std::filesystem::path projectPath_;
};

inline SPathManager PathManager;
