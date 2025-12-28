#pragma once
#include <unordered_map>
#include <filesystem>

class KtTexture;

class KtTextureManager final
{
	friend class SCore;

private:
	void Cleanup(); 

public:
	KtTexture* Get(const std::filesystem::path& path);

private:
	std::unordered_map<std::filesystem::path, KtTexture*> textures_;

	KtTexture* Create(const std::filesystem::path& path);
};

inline KtTextureManager TextureManager;
