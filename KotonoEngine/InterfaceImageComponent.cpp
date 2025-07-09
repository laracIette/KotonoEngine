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
		WireframeShader = Framework.GetShaderManager().Get(path);
		WireframeShader->SetName("2D Wireframe Shader");
	}
}

void KInterfaceImageComponent::Init()
{
	Base::Init();
	InitImageTextureProxy();
}

void KInterfaceImageComponent::Update()
{
	Base::Update();
}

void KInterfaceImageComponent::Cleanup()
{
	Base::Cleanup();
	Framework.GetRenderer().GetRenderer2D().Remove(&imageTextureProxy_);
}

KtShader* KInterfaceImageComponent::GetShader() const
{
	return shader_;
}

KtImageTexture* KInterfaceImageComponent::GetImageTexture() const
{
	return imageTexture_;
}

KtEvent<>& KInterfaceImageComponent::GetEventShaderUpdated()
{
	return eventShaderUpdated_;
}

KtEvent<>& KInterfaceImageComponent::GetEventImageTextureUpdated()
{
	return eventImageTextureUpdated_;
}

void KInterfaceImageComponent::SetShader(KtShader* shader)
{
	shader_ = shader;
	eventShaderUpdated_.Broadcast();
}

void KInterfaceImageComponent::SetImageTexture(KtImageTexture* imageTexture)
{
	imageTexture_ = imageTexture;
	eventImageTextureUpdated_.Broadcast();
}

void KInterfaceImageComponent::InitImageTextureProxy()
{
	CreateImageTextureProxy();
	Framework.GetRenderer().GetRenderer2D().Register(&imageTextureProxy_);

	const KtDelegate<> rectDelegate(this, &KInterfaceImageComponent::MarkImageTextureProxyRectDirty);
	const KtDelegate<> shaderDelegate(this, &KInterfaceImageComponent::MarkImageTextureProxyShaderDirty);
	const KtDelegate<> imageTextureDelegate(this, &KInterfaceImageComponent::MarkImageTextureProxyImageTextureDirty);
	ListenEvent(GetEventRectUpdated(), rectDelegate);
	ListenEvent(GetEventShaderUpdated(), shaderDelegate);
	ListenEvent(GetEventImageTextureUpdated(), imageTextureDelegate);
}

void KInterfaceImageComponent::CreateImageTextureProxy()
{
	imageTextureProxy_.shader = GetShader();
	imageTextureProxy_.renderable = GetImageTexture();
	imageTextureProxy_.viewport = GetOwner()->GetViewport();
	imageTextureProxy_.layer = GetLayer();
	imageTextureProxy_.objectData.modelMatrix = GetModelMatrix();
}

void KInterfaceImageComponent::MarkImageTextureProxyRectDirty()
{
	imageTextureProxy_.isDirty = true;
	imageTextureProxy_.objectData.modelMatrix = GetModelMatrix();
}

void KInterfaceImageComponent::MarkImageTextureProxyShaderDirty()
{
	imageTextureProxy_.isDirty = true;
	imageTextureProxy_.shader = GetShader();
}

void KInterfaceImageComponent::MarkImageTextureProxyImageTextureDirty()
{
	imageTextureProxy_.isDirty = true;
	imageTextureProxy_.renderable = GetImageTexture();
}
