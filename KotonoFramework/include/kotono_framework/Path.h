#pragma once
#include <filesystem>
class KtPath final
{
	friend class KtFramework;

private:
	void Init();

public:
	const std::filesystem::path& RootPath() const;
	const std::filesystem::path& FrameworkPath() const;
	const std::filesystem::path& EnginePath() const;

private:
	std::filesystem::path rootPath_;
	std::filesystem::path frameworkPath_;
	std::filesystem::path enginePath_;
};

