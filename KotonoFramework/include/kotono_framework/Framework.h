#pragma once
class KtFramework final
{
public:
	void Init();
	void Cleanup();

	class KtPath& GetPath();
	class KtWindow& GetWindow();
	class KtContext& GetContext();
	class KtRenderer& GetRenderer();
	class KtAudioManager& GetAudioManager();
	class KtInputManager& GetInputManager();
	class KtModelManager& GetModelManager();
	class KtShaderManager& GetShaderManager();
	class KtImageTextureManager& GetImageTextureManager();
};

extern KtFramework Framework;
