#pragma once
#include "ItemManager.h"
#include "Model.h"
class KtModelManager final : public KtItemManager<KtModel>
{
public:
	KtModel* CreateItem(const std::filesystem::path& path) const override;
	void CleanupItem(KtModel* item) const override;
};

