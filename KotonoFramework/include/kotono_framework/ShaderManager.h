#pragma once
#include "ItemManager.h"
#include "Shader.h"
class KtShaderManager final : public KtItemManager<KtShader>
{
public:
	KtShader* CreateItem(const std::filesystem::path& path) const override;
	void CleanupItem(KtShader* item) const override;
};

