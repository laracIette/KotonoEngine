#include "Framework.h"
#include "Path.h"
#include "Window.h"
#include "Context.h"
#include "Renderer.h"
#include "AudioManager.h"
#include "InputManager.h"
#include "ModelManager.h"
#include "ShaderManager.h"
#include "ImageTextureManager.h"

static KtPath Path;
static KtWindow Window;
static KtContext Context;
static KtRenderer Renderer;
static KtAudioManager AudioManager;
static KtInputManager InputManager;
static KtModelManager ModelManager;
static KtShaderManager ShaderManager;
static KtImageTextureManager ImageTextureManager;

void KtFramework::Init()
{
    ::Path.Init();
    ::Window.Init();
    ::Context.Init();
    ::Renderer.Init();
    ::AudioManager.Init();
    ::InputManager.Init();
    ::ShaderManager.Init();
}

void KtFramework::Update()
{
    ::InputManager.Update();
}

void KtFramework::Cleanup()
{
    ::ImageTextureManager.Cleanup();
    ::ShaderManager.Cleanup();
    ::ModelManager.Cleanup();
    ::AudioManager.Cleanup();
    ::Renderer.Cleanup();
    ::Context.Cleanup();
    ::Window.Cleanup();
}

KtPath& KtFramework::Path()
{
    return ::Path;
}

const KtPath& KtFramework::Path() const
{
    return ::Path;
}

KtWindow& KtFramework::Window()
{
    return ::Window;
}

const KtWindow& KtFramework::Window() const
{
    return ::Window;
}

KtContext& KtFramework::Context()
{
    return ::Context;
}

const KtContext& KtFramework::Context() const
{
    return ::Context;
}

KtRenderer& KtFramework::Renderer()
{
    return ::Renderer;
}

const KtRenderer& KtFramework::Renderer() const
{
    return ::Renderer;
}

KtAudioManager& KtFramework::AudioManager()
{
    return ::AudioManager;
}

const KtAudioManager& KtFramework::AudioManager() const
{
    return ::AudioManager;
}

KtInputManager& KtFramework::InputManager()
{
    return ::InputManager;
}

const KtInputManager& KtFramework::InputManager() const
{
    return ::InputManager;
}

KtModelManager& KtFramework::ModelManager()
{
    return ::ModelManager;
}

const KtModelManager& KtFramework::ModelManager() const
{
    return ::ModelManager;
}

KtShaderManager& KtFramework::ShaderManager()
{
    return ::ShaderManager;
}

const KtShaderManager& KtFramework::ShaderManager() const
{
    return ::ShaderManager;
}

KtImageTextureManager& KtFramework::ImageTextureManager()
{
    return ::ImageTextureManager;
}

const KtImageTextureManager& KtFramework::ImageTextureManager() const
{
    return ::ImageTextureManager;
}
