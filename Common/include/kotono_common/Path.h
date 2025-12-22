#pragma once
#include <filesystem>
class KtPath final
{
public:
	static std::filesystem::path Graphics();
	static const std::filesystem::path& Project();

private:
	static std::filesystem::path projectPath_;
};
