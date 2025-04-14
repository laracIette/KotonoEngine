#pragma once
#include "ItemManager.h"
#include "ModelWireframe.h"
class KtModelWireframeManager final : public KtItemManager<KtModelWireframe>
{
public:
	KtModelWireframe* Create(const std::filesystem::path& path) override;

protected:
	void CleanupItem(KtModelWireframe* item) const override;
};

