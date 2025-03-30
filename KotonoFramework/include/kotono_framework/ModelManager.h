#pragma once
#include "ItemManager.h"
#include "Model.h"
class KtModelManager final : public KtItemManager<KtModel>
{
public:
	KtModel* Create(const std::filesystem::path& path) override;

protected:
	void CleanupItem(KtModel* item) const override;
};

