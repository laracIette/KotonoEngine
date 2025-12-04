#pragma once
#include <unordered_map>
#include <filesystem>

class KtImageTexture;

class KtImageTextureManager final
{
	friend class KtFramework;

private:
	void Cleanup(); 

public:
	KtImageTexture* Get(const std::filesystem::path& path);

private:
	std::unordered_map<std::filesystem::path, KtImageTexture*> imageTextures_;

	KtImageTexture* Create(const std::filesystem::path& path);
};

