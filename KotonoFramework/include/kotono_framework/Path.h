#pragma once
#include <filesystem>
class KtPath
{
public:
	void Init();
	void Cleanup();

	const std::filesystem::path& GetSolutionPath() const;
	const std::filesystem::path& GetFrameworkPath() const;
	const std::filesystem::path& GetEnginePath() const;

private:
	std::filesystem::path _solutionPath;
	std::filesystem::path _frameworkPath;
	std::filesystem::path _enginePath;
};

