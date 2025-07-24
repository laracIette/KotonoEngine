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
#include "log.h"

static KtShader* WireframeShader{ nullptr };

KInterfaceImageComponent::KInterfaceImageComponent(RInterfaceObject* owner) :
	Base(owner)
{
	if (!WireframeShader)
	{
		const auto path{ Framework.GetPath().GetFrameworkPath() / R"(shaders\wireframe2D.ktshader)" };
		WireframeShader = Framework.GetShaderManager().Get(path);
		WireframeShader->SetName("2D Wireframe Shader");
	}
}

void KInterfaceImageComponent::Init()
{
	Base::Init();

	Framework.GetRenderer().GetInterfaceRenderer().Register(&imageTextureProxy_);
	
	CreateImageTextureProxy();

	GetEventRectUpdated().AddListener(KtDelegate(this, &KInterfaceImageComponent::MarkImageTextureProxyRectDirty));
	GetEventShaderUpdated().AddListener(KtDelegate(this, &KInterfaceImageComponent::MarkImageTextureProxyShaderDirty));
	GetEventImageTextureUpdated().AddListener(KtDelegate(this, &KInterfaceImageComponent::MarkImageTextureProxyImageTextureDirty));
	GetEventLayerUpdated().AddListener(KtDelegate(this, &KInterfaceImageComponent::MarkImageTextureProxyLayerDirty));
}

void KInterfaceImageComponent::Update()
{
	Base::Update();
}

void KInterfaceImageComponent::Cleanup()
{
	Base::Cleanup();

	Framework.GetRenderer().GetInterfaceRenderer().Unregister(&imageTextureProxy_);
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

void KInterfaceImageComponent::MarkImageTextureProxyLayerDirty()
{
	imageTextureProxy_.isDirty = true;
	imageTextureProxy_.layer = GetLayer();
}
