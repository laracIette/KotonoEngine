#pragma once
#include "ItemManager.h"
#include "ImageTexture.h"

class KtImageTextureManager final : public KtItemManager<KtImageTexture>
{
public:
	KtImageTexture* Create(const std::filesystem::path& path) override;

protected:
	void CleanupItem(KtImageTexture* item) const override;
};

