#pragma once
#include <unordered_map>
#include <filesystem>
#include <kotono_common/Path.h>

class KtShader;

class KtShaderManager final
{
	friend class SCore;

private:
	void Init(); 
	void Cleanup();

public:
	KtShader* Get(const UPath& path);

private:
	KtShader* Create(const UPath& path);

private:
	std::unordered_map<UPath, KtShader*> shaders_;
};

inline KtShaderManager ShaderManager;
