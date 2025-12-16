#pragma once
#include <filesystem>
class KtPath final
{
	friend class SCore;
	friend void init_project_path();

private:
	void Init();

public:
	const std::filesystem::path& Root() const;
	const std::filesystem::path& Framework() const;
	const std::filesystem::path& Engine() const;
	const std::filesystem::path& Project() const;

private:
	std::filesystem::path rootPath_;
	std::filesystem::path frameworkPath_;
	std::filesystem::path enginePath_;
	std::filesystem::path projectPath_;
};

inline KtPath Path;
