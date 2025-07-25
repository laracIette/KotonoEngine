#include "Framework.h"

void KtFramework::Init()
{
    path_.Init();
    window_.Init();
    context_.Init();
    renderer_.Init();
    audioManager_.Init();
    inputManager_.Init();
    shaderManager_.Init();
}

void KtFramework::Update()
{
    inputManager_.Update();
}

void KtFramework::Cleanup()
{
    imageTextureManager_.Cleanup();
    shaderManager_.Cleanup();
    modelManager_.Cleanup();
    audioManager_.Cleanup();
    renderer_.Cleanup();
    context_.Cleanup();
    window_.Cleanup();
}

KtPath& KtFramework::GetPath()
{
    return path_;
}

const KtPath& KtFramework::GetPath() const
{
    return path_;
}

KtWindow& KtFramework::GetWindow()
{
    return window_;
}

const KtWindow& KtFramework::GetWindow() const
{
    return window_;
}

KtContext& KtFramework::GetContext()
{
    return context_;
}

const KtContext& KtFramework::GetContext() const
{
    return context_;
}

KtRenderer& KtFramework::GetRenderer()
{
    return renderer_;
}

const KtRenderer& KtFramework::GetRenderer() const
{
    return renderer_;
}

KtAudioManager& KtFramework::GetAudioManager()
{
    return audioManager_;
}

const KtAudioManager& KtFramework::GetAudioManager() const
{
    return audioManager_;
}

KtInputManager& KtFramework::GetInputManager()
{
    return inputManager_;
}

const KtInputManager& KtFramework::GetInputManager() const
{
    return inputManager_;
}

KtModelManager& KtFramework::GetModelManager()
{
    return modelManager_;
}

const KtModelManager& KtFramework::GetModelManager() const
{
    return modelManager_;
}

KtShaderManager& KtFramework::GetShaderManager()
{
    return shaderManager_;
}

const KtShaderManager& KtFramework::GetShaderManager() const
{
    return shaderManager_;
}

KtImageTextureManager& KtFramework::GetImageTextureManager()
{
    return imageTextureManager_;
}

const KtImageTextureManager& KtFramework::GetImageTextureManager() const
{
    return imageTextureManager_;
}
