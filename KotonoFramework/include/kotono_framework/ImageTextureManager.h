#pragma once
#include "ItemManager.h"
#include "ImageTexture.h"

class KtImageTextureManager final : public KtItemManager<KtImageTexture>
{
protected:
	KtImageTexture* Create(const std::filesystem::path& path) override;
	void CleanupItem(KtImageTexture* item) const override;
};

