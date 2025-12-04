#include "InterfaceImageComponent.h"
#include <kotono_framework/Framework.h>
#include <kotono_framework/ShaderManager.h>
#include <kotono_common/Path.h>
#include <kotono_framework/Renderer.h>
#include <kotono_framework/Shader.h>
#include <kotono_framework/ImageTexture.h>
#include <kotono_framework/InterfaceRenderableProxy.h>
#include <kotono_framework/WindowViewport.h>
#include "InterfaceObject.h"
#include "log.h"

static constinit KtShader* WireframeShader{ nullptr };

KInterfaceImageComponent::KInterfaceImageComponent(UPtrOwnerBase* ptrOwner, const UPtr<RInterfaceObject>& owner) :
	Base(ptrOwner, owner)
{
	if (!WireframeShader)
	{
		static const auto path{ Framework.Path().Framework() / R"(shaders\wireframe2D.ktshader)" };
		WireframeShader = Framework.ShaderManager().Get(path);
		WireframeShader->SetName("2D Wireframe Shader");
	}

	imageTextureProxy_ = Framework.Renderer().GetInterfaceRenderer().CreateProxy();
}

void KInterfaceImageComponent::Init()
{
	Base::Init();

	Framework.Renderer().GetInterfaceRenderer().Register(imageTextureProxy_);
	CreateImageTextureProxy();

	EventRectUpdated().AddListener(KtDelegate(this, &KInterfaceImageComponent::MarkImageTextureProxyRectDirty));
	GetEventShaderUpdated().AddListener(KtDelegate(this, &KInterfaceImageComponent::MarkImageTextureProxyShaderDirty));
	GetEventImageTextureUpdated().AddListener(KtDelegate(this, &KInterfaceImageComponent::MarkImageTextureProxyImageTextureDirty));
	EventLayerUpdated().AddListener(KtDelegate(this, &KInterfaceImageComponent::MarkImageTextureProxyLayerDirty));
}

void KInterfaceImageComponent::Cleanup()
{
	Base::Cleanup();

	Framework.Renderer().GetInterfaceRenderer().Unregister(imageTextureProxy_);
	Framework.Renderer().GetInterfaceRenderer().DeleteProxy(imageTextureProxy_);
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
	imageTextureProxy_->scissor.offset = Owner()->GetViewport()->GetOffset();
	imageTextureProxy_->scissor.extent = Owner()->GetViewport()->GetExtent();
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
