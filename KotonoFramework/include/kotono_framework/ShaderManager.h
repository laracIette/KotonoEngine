#pragma once
#include "ItemManager.h"
#include "Shader.h"

class KtShaderManager final : public KtItemManager<KtShader>
{
public:
	KtShader* Create(const std::filesystem::path& path) override;

protected:
	void CleanupItem(KtShader* item) const override;
};

