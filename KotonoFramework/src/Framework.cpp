#include "Framework.h"
#include "Path.h"
#include "Window.h"
#include "Context.h"
#include "Renderer.h"
#include "AudioManager.h"
#include "InputManager.h"
#include "ModelManager.h"
#include "ModelWireframeManager.h"
#include "ShaderManager.h"
#include "ImageTextureManager.h"

KtFramework Framework;

static KtPath Path;
static KtWindow Window;
static KtContext Context;
static KtRenderer Renderer;
static KtAudioManager AudioManager;
static KtInputManager InputManager;
static KtModelManager ModelManager;
static KtModelWireframeManager ModelWireframeManager;
static KtShaderManager ShaderManager;
static KtImageTextureManager ImageTextureManager;

void KtFramework::Init()
{
    Path.Init();
    Window.Init();
    Context.Init();
    Renderer.Init();
    AudioManager.Init();
    InputManager.Init();
    ModelManager.Init();
    ModelWireframeManager.Init();
    ShaderManager.Init();
    ImageTextureManager.Init();
}

void KtFramework::Cleanup()
{
    ImageTextureManager.Cleanup();
    ShaderManager.Cleanup();
    ModelWireframeManager.Cleanup();
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

KtWindow& KtFramework::GetWindow()
{
    return Window;
}

KtContext& KtFramework::GetContext()
{
    return Context;
}

KtRenderer& KtFramework::GetRenderer()
{
    return Renderer;
}

KtAudioManager& KtFramework::GetAudioManager()
{
    return AudioManager;
}

KtInputManager& KtFramework::GetInputManager()
{
    return InputManager;
}

KtModelManager& KtFramework::GetModelManager()
{
    return ModelManager;
}

KtModelWireframeManager& KtFramework::GetModelWireframeManager()
{
    return ModelWireframeManager;
}

KtShaderManager& KtFramework::GetShaderManager()
{
    return ShaderManager;
}

KtImageTextureManager& KtFramework::GetImageTextureManager()
{
    return ImageTextureManager;
}
