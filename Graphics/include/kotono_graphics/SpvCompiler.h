#pragma once
#include <filesystem>
class USpvCompiler final
{
public:
	static void CompileAll();
	static void CompileUpdated();

private:
	static bool DependenciesUpdated();
	static bool Compile(const std::filesystem::path& path);
};

