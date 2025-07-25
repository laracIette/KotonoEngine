#pragma once
#include "Path.h"
#include "Window.h"
#include "Context.h"
#include "Renderer.h"
#include "AudioManager.h"
#include "InputManager.h"
#include "ModelManager.h"
#include "ShaderManager.h"
#include "ImageTextureManager.h"

class KtFramework final
{
	friend class SApplication; // todo: eh ?

private:
	void Init();
	void Update();
	void Cleanup();

public:
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

private:
	KtPath path_;
	KtWindow window_;
	KtContext context_;
	KtRenderer renderer_;
	KtAudioManager audioManager_;
	KtInputManager inputManager_;
	KtModelManager modelManager_;
	KtShaderManager shaderManager_;
	KtImageTextureManager imageTextureManager_;
};

extern KtFramework Framework;
