#pragma once
#include <filesystem>
class SSpvCompiler final
{
public:
	static void CompileAll();
	static void CompileUpdated();

private:
	static bool DependenciesUpdated();
	static bool Compile(std::filesystem::path const& path);
};

