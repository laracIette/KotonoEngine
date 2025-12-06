#pragma once
class KtPath;
class KtRenderer;
class KtTimeManager;
class KtModelManager;
class KtShaderManager;
class KtImageTextureManager;
class KtFramework final
{
public:
	void Init();
	void Update();
	void Cleanup();

public:
	KtPath& Path();
	const KtPath& Path() const;
	KtRenderer& Renderer();
	const KtRenderer& Renderer() const;
	KtTimeManager& TimeManager();
	const KtTimeManager& TimeManager() const;
	KtModelManager& ModelManager();
	const KtModelManager& ModelManager() const;
	KtShaderManager& ShaderManager();
	const KtShaderManager& ShaderManager() const;
	KtImageTextureManager& ImageTextureManager();
	const KtImageTextureManager& ImageTextureManager() const;
};

inline KtFramework Framework;
