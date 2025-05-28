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
#include "SpvCompiler.h"

KtFramework Framework;

static KtPath Path;
static KtSpvCompiler SpvCompiler;
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
    Path.Init();
    SpvCompiler.CompileUpdated(); // todo: move to shader manager ?
    Window.Init();
    Context.Init();
    Renderer.Init();
    AudioManager.Init();
    InputManager.Init();
    ModelManager.Init();
    ShaderManager.Init();
    ImageTextureManager.Init();
}

void KtFramework::Cleanup()
{
    ImageTextureManager.Cleanup();
    ShaderManager.Cleanup();
    ModelManager.Cleanup();
    InputManager.Cleanup();
    AudioManager.Cleanup();
    Renderer.Cleanup();
    Context.Cleanup();
    Window.Cleanup();
    Path.Cleanup();
}

KtPath& KtFramework::GetPath()
{
    return Path;
}

const KtPath& KtFramework::GetPath() const
{
    return Path;
}

KtWindow& KtFramework::GetWindow()
{
    return Window;
}

const KtWindow& KtFramework::GetWindow() const
{
    return Window;
}

KtContext& KtFramework::GetContext()
{
    return Context;
}

const KtContext& KtFramework::GetContext() const
{
    return Context;
}

KtRenderer& KtFramework::GetRenderer()
{
    return Renderer;
}

const KtRenderer& KtFramework::GetRenderer() const
{
    return Renderer;
}

KtAudioManager& KtFramework::GetAudioManager()
{
    return AudioManager;
}

const KtAudioManager& KtFramework::GetAudioManager() const
{
    return AudioManager;
}

KtInputManager& KtFramework::GetInputManager()
{
    return InputManager;
}

const KtInputManager& KtFramework::GetInputManager() const
{
    return InputManager;
}

KtModelManager& KtFramework::GetModelManager()
{
    return ModelManager;
}

const KtModelManager& KtFramework::GetModelManager() const
{
    return ModelManager;
}

KtShaderManager& KtFramework::GetShaderManager()
{
    return ShaderManager;
}

const KtShaderManager& KtFramework::GetShaderManager() const
{
    return ShaderManager;
}

KtImageTextureManager& KtFramework::GetImageTextureManager()
{
    return ImageTextureManager;
}

const KtImageTextureManager& KtFramework::GetImageTextureManager() const
{
    return ImageTextureManager;
}
