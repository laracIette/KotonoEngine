#pragma once
#include "ItemManager.h"
#include "ImageTexture.h"
class KtImageTextureManager final : public KtItemManager<KtImageTexture>
{
public:
	KtImageTexture* CreateItem(const std::filesystem::path& path) const override;
	void CleanupItem(KtImageTexture* item) const override;
};

