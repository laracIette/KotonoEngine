#pragma once
#include "ItemManager.h"
#include "Model.h"

class KtModelManager final : public KtItemManager<KtModel>
{
protected:
	KtModel* Create(const std::filesystem::path& path) override;
	void CleanupItem(KtModel* item) const override;
};

