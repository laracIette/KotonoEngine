#include "InterfaceImageComponent.h"
#include <kotono_graphics/ShaderManager.h>
#include <kotono_common/Path.h>
#include <kotono_graphics/Renderer.h>
#include <kotono_graphics/Shader.h>
#include <kotono_graphics/ImageTexture.h>
#include <kotono_graphics/InterfaceRenderableProxy.h>
#include <kotono_platform/WindowViewport.h>
#include "InterfaceObject.h"
#include <kotono_common/log.h>

static constinit KtShader* WireframeShader{ nullptr };

KInterfaceImageComponent::KInterfaceImageComponent(UPtrOwnerBase* ptrOwner) :
	Base(ptrOwner)
{
	if (!WireframeShader)
	{
		static const auto path{ KtPath::Graphics() / R"(shaders\wireframe2D.ktshader)" };
		WireframeShader = ShaderManager.Get(path);
		WireframeShader->SetName("2D Wireframe Shader");
	}

	imageTextureProxy_ = Renderer.InterfaceRenderer().CreateProxy();
}

void KInterfaceImageComponent::Cleanup()
{
	Base::Cleanup();

	Renderer.InterfaceRenderer().Unregister(imageTextureProxy_);
	Renderer.InterfaceRenderer().DeleteProxy(imageTextureProxy_);
}

void KInterfaceImageComponent::Init()
{
	Base::Init();

	Renderer.InterfaceRenderer().Register(imageTextureProxy_);
	CreateImageTextureProxy();

	EventRectUpdated().AddListener(KtDelegate(this, &KInterfaceImageComponent::MarkImageTextureProxyRectDirty));
	GetEventShaderUpdated().AddListener(KtDelegate(this, &KInterfaceImageComponent::MarkImageTextureProxyShaderDirty));
	GetEventImageTextureUpdated().AddListener(KtDelegate(this, &KInterfaceImageComponent::MarkImageTextureProxyImageTextureDirty));
	EventLayerUpdated().AddListener(KtDelegate(this, &KInterfaceImageComponent::MarkImageTextureProxyLayerDirty));
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
	imageTextureProxy_->shader = GetShader();
	imageTextureProxy_->renderable = GetImageTexture();
	imageTextureProxy_->layer = GetLayer();
	imageTextureProxy_->objectData.modelMatrix = ModelMatrix();
	imageTextureProxy_->scissor.offset = GetOwner()->GetViewport()->GetOffset();
	imageTextureProxy_->scissor.extent = GetOwner()->GetViewport()->GetExtent();
#	ifdef _DEBUG
		imageTextureProxy_->source = this;
#	endif
}

void KInterfaceImageComponent::MarkImageTextureProxyRectDirty()
{
	imageTextureProxy_->isDirty = true;
	imageTextureProxy_->objectData.modelMatrix = ModelMatrix();
}

void KInterfaceImageComponent::MarkImageTextureProxyShaderDirty()
{
	imageTextureProxy_->isDirty = true;
	imageTextureProxy_->shader = GetShader();
}

void KInterfaceImageComponent::MarkImageTextureProxyImageTextureDirty()
{
	imageTextureProxy_->isDirty = true;
	imageTextureProxy_->renderable = GetImageTexture();
}

void KInterfaceImageComponent::MarkImageTextureProxyLayerDirty()
{
	imageTextureProxy_->isDirty = true;
	imageTextureProxy_->layer = GetLayer();
}
