#include "InterfaceImageComponent.h"
#include <kotono_framework/Framework.h>
#include <kotono_framework/ShaderManager.h>
#include <kotono_framework/Path.h>
#include <kotono_framework/Renderer.h>
#include <kotono_framework/Shader.h>
#include <kotono_framework/ImageTexture.h>
#include "Engine.h"
#include "ObjectManager.h"
#include "InterfaceObject.h"

static KtShader* WireframeShader = nullptr;

void KInterfaceImageComponent::Construct()
{
	Base::Construct();

	if (!WireframeShader)
	{
		const auto path = Framework.GetPath().GetFrameworkPath() / R"(shaders\wireframe2D.ktshader)";
		WireframeShader = Framework.GetShaderManager().Create(path);
		WireframeShader->SetName("2D Wireframe Shader");
	}
}

void KInterfaceImageComponent::Init()
{
	Base::Init();

	ListenEvent(Engine.GetObjectManager().GetEventDrawInterfaceObjects(), &KInterfaceImageComponent::AddTextureToRenderQueue);
	ListenEvent(Engine.GetObjectManager().GetEventDrawInterfaceObjectWireframes(), &KInterfaceImageComponent::AddWireframeToRenderQueue);
}

void KInterfaceImageComponent::Update()
{
	Base::Update();
}

void KInterfaceImageComponent::Cleanup()
{
	Base::Cleanup();
}

KtShader* KInterfaceImageComponent::GetShader() const
{
	return shader_;
}

KtImageTexture* KInterfaceImageComponent::GetImageTexture() const
{
	return _imageTexture;
}

void KInterfaceImageComponent::SetShader(KtShader* shader)
{
	shader_ = shader;
}

void KInterfaceImageComponent::SetImageTexture(KtImageTexture* imageTexture)
{
	_imageTexture = imageTexture;
}

void KInterfaceImageComponent::AddTextureToRenderQueue()
{
	KtAddToRenderQueue2DArgs args{};
	args.Shader = shader_;
	args.Renderable = _imageTexture;
	args.Viewport = GetOwner()->GetViewport();
	args.ObjectData.Model = GetRect().GetModelMatrix();
	args.Layer = GetLayer();
	Framework.GetRenderer().GetRenderer2D().AddToRenderQueue(args);
}

void KInterfaceImageComponent::AddWireframeToRenderQueue()
{
	KtAddToRenderQueue2DArgs args{};
	args.Shader = WireframeShader;
	args.Renderable = _imageTexture;
	args.Viewport = GetOwner()->GetViewport();
	args.ObjectData.Model = GetRect().GetModelMatrix();
	args.Layer = GetLayer();
	Framework.GetRenderer().GetRenderer2D().AddToRenderQueue(args);
}
