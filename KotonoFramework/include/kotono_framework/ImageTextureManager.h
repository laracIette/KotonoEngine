#pragma once
#include <unordered_map>
#include "ImageTexture.h"
class KtImageTextureManager final
{
public:
	KtImageTexture* Get(const std::filesystem::path& path);

	void Init();
	void Cleanup();

private:
	std::unordered_map<std::filesystem::path, KtImageTexture*> _imageTextures;
};

