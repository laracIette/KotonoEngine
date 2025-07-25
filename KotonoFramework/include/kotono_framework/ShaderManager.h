#pragma once
#include <unordered_map>
#include <filesystem>

class KtShader;

class KtShaderManager final
{
public:
	void Init(); // todo: make private and friend
	void Cleanup();

	KtShader* Get(const std::filesystem::path& path);

private:
	std::unordered_map<std::filesystem::path, KtShader*> shaders_;

	KtShader* Create(const std::filesystem::path& path);
};

