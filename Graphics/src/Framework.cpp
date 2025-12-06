#include "Framework.h"
#include <kotono_common/Path.h>
#include <kotono_platform/Window.h>
#include <kotono_platform/Context.h>
#include "Renderer.h"
#include <kotono_timing/TimeManager.h>
#include "ModelManager.h"
#include "ShaderManager.h"
#include "ImageTextureManager.h"

static KtPath Path;
static KtRenderer Renderer;
static KtTimeManager TimeManager;
static KtModelManager ModelManager;
static KtShaderManager ShaderManager;
static KtImageTextureManager ImageTextureManager;

void KtFramework::Init()
{
    ::Path.Init();
    Window.Init();
    Context.Init();
    ::Renderer.Init();
    ::ShaderManager.Init();

    auto& renderTimer{ ::TimeManager.GetTimer("render") };
    renderTimer.SetDuration(1.0f / 60.0f);
    renderTimer.SetIsRepeat(true);
    renderTimer.EventCompleted().AddListener(KtDelegate(&::Renderer, &KtRenderer::DrawFrame));
    renderTimer.Start();
}

void KtFramework::Update()
{
    ::TimeManager.Update();
}

void KtFramework::Cleanup()
{
    ::ImageTextureManager.Cleanup();
    ::ShaderManager.Cleanup();
    ::ModelManager.Cleanup();
    ::Renderer.Cleanup();
    Context.Cleanup();
    Window.Cleanup();
}

KtPath& KtFramework::Path()
{
    return ::Path;
}

const KtPath& KtFramework::Path() const
{
    return ::Path;
}

KtRenderer& KtFramework::Renderer()
{
    return ::Renderer;
}

const KtRenderer& KtFramework::Renderer() const
{
    return ::Renderer;
}

KtTimeManager& KtFramework::TimeManager()
{
    return ::TimeManager;
}

const KtTimeManager& KtFramework::TimeManager() const
{
    return ::TimeManager;
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