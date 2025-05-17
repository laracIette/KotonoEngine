#pragma once
#include <filesystem>
class KtPath final
{
public:
	void Init();
	void Cleanup();

	const std::filesystem::path& GetSolutionPath() const;
	const std::filesystem::path& GetFrameworkPath() const;
	const std::filesystem::path& GetEnginePath() const;

private:
	std::filesystem::path solutionPath_;
	std::filesystem::path frameworkPath_;
	std::filesystem::path enginePath_;
};

