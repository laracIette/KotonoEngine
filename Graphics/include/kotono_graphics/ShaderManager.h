#pragma once
#include <unordered_map>
#include <filesystem>

class KtShader;

class KtShaderManager final
{
	friend class SEngine;

private:
	void Init(); 
	void Cleanup();

public:
	KtShader* Get(const std::filesystem::path& path);

private:
	std::unordered_map<std::filesystem::path, KtShader*> shaders_;

	KtShader* Create(const std::filesystem::path& path);
};

inline KtShaderManager ShaderManager;
