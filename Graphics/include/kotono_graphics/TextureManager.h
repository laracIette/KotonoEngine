#pragma once
#include <unordered_map>
#include <filesystem>
#include <kotono_common/Path.h>

class KtTexture;

class KtTextureManager final
{
	friend class SCore;

private:
	void Cleanup(); 

public:
	KtTexture* Get(const UPath& path);

private:
	KtTexture* Create(const UPath& path);

private:
	std::unordered_map<UPath, KtTexture*> textures_;
};

inline KtTextureManager TextureManager;
