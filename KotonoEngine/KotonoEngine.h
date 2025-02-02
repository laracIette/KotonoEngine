#pragma once
#include <filesystem>
class KotonoEngine final
{
public:
	KotonoEngine();

	const std::filesystem::path& GetProjectDirectory() const;

private:
	std::filesystem::path _projectDirectory;
};

extern KotonoEngine Engine;

