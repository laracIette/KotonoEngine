#pragma once
#include <filesystem>
class SPathManager final
{
public:
	static const std::filesystem::path& Engine();
	static const std::filesystem::path& Project();

private:
	static std::filesystem::path projectPath_;
};
