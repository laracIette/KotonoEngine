#pragma once
#include "ItemManager.h"
#include "Shader.h"

class KtShaderManager final : public KtItemManager<KtShader>
{
public:
	void Init();

protected:
	KtShader* Create(const std::filesystem::path& path) override;
	void CleanupItem(KtShader* item) const override;
};

