#pragma once
class KtClock;
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
	void Cleanup();

public:
	KtClock& Clock();
	const KtClock& Clock() const;
	KtPath& Path();
	const KtPath& Path() const;
	KtWindow& Window();
	const KtWindow& Window() const;
	KtContext& Context();
	const KtContext& Context() const;
	KtRenderer& Renderer();
	const KtRenderer& Renderer() const;
	KtAudioManager& AudioManager();
	const KtAudioManager& AudioManager() const;
	KtInputManager& InputManager();
	const KtInputManager& InputManager() const;
	KtModelManager& ModelManager();
	const KtModelManager& ModelManager() const;
	KtShaderManager& ShaderManager();
	const KtShaderManager& ShaderManager() const;
	KtImageTextureManager& ImageTextureManager();
	const KtImageTextureManager& ImageTextureManager() const;
};

inline KtFramework Framework;
