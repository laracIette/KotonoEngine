#pragma once
#include <Path/Path.h>
#include <types.h>
class SSpvCompiler final
{
public:
	static void CompileAll();
	static void CompileUpdated();

private:
	static auto HasDependenciesUpdated() -> b8;
	static auto Compile(UPath const& path) -> b8;
};

