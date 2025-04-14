#pragma once
class KtPath;
class KtWindow;
class KtContext;
class KtRenderer;
class KtAudioManager;
class KtInputManager;
class KtModelManager;
class KtModelWireframeManager;
class KtShaderManager;
class KtImageTextureManager;
class KtFramework final
{
public:
	void Init();
	void Cleanup();

	KtPath& GetPath();
	KtWindow& GetWindow();
	KtContext& GetContext();
	KtRenderer& GetRenderer();
	KtAudioManager& GetAudioManager();
	KtInputManager& GetInputManager();
	KtModelManager& GetModelManager();
	KtModelWireframeManager& GetModelWireframeManager();
	KtShaderManager& GetShaderManager();
	KtImageTextureManager& GetImageTextureManager();
};

extern KtFramework Framework;
