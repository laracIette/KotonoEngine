#pragma once
#include <unordered_map>
#include <filesystem>

class KtImageTexture;

class KtImageTextureManager final
{
	friend class SCore;

private:
	void Cleanup(); 

public:
	KtImageTexture* Get(const std::filesystem::path& path);

private:
	std::unordered_map<std::filesystem::path, KtImageTexture*> imageTextures_;

	KtImageTexture* Create(const std::filesystem::path& path);
};

inline KtImageTextureManager ImageTextureManager;
