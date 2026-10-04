#pragma once
#include <filesystem>
class SSpvCompiler final
{
public:
	static void CompileAll();
	static void CompileUpdated();

private:
	static bool DependenciesUpdated();
	static bool Compile(const std::filesystem::path& path);
};

