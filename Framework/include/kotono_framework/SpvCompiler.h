#pragma once
#include <filesystem>

class KtSpvCompiler final
{
public:
	void CompileUpdated() const;
	void CompileAll() const;

private:
	void Compile(const std::filesystem::path& path) const;
};

