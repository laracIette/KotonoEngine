#pragma once

class KtPath;
class KtWindow;
class KtContext;
class KtRenderer;
class KtAudioManager;
class KtInputManager;
class KtModelManager;
class KtShaderManager;
class KtImageTextureManager;

class KtFramework final
{
public:
	void Init();
	void Update();
	void Cleanup();

	KtPath& GetPath();
	const KtPath& GetPath() const;
	KtWindow& GetWindow();
	const KtWindow& GetWindow() const;
	KtContext& GetContext();
	const KtContext& GetContext() const;
	KtRenderer& GetRenderer();
	const KtRenderer& GetRenderer() const;
	KtAudioManager& GetAudioManager();
	const KtAudioManager& GetAudioManager() const;
	KtInputManager& GetInputManager();
	const KtInputManager& GetInputManager() const;
	KtModelManager& GetModelManager();
	const KtModelManager& GetModelManager() const;
	KtShaderManager& GetShaderManager();
	const KtShaderManager& GetShaderManager() const;
	KtImageTextureManager& GetImageTextureManager();
	const KtImageTextureManager& GetImageTextureManager() const;
};

extern KtFramework Framework;
