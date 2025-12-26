#include "InterfaceImageComponent.h"
#include <kotono_graphics/ShaderManager.h>
#include <kotono_common/Path.h>
#include <kotono_graphics/Renderer.h>
#include <kotono_graphics/Shader.h>
#include <kotono_graphics/ImageTexture.h>
#include <kotono_graphics/InterfaceProxy.h>
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

	Renderer.InterfaceRenderer().UnregisterProxy(imageTextureProxy_);
	Renderer.InterfaceRenderer().DeleteProxy(imageTextureProxy_);
}

void KInterfaceImageComponent::Init()
{
	Base::Init();
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

void KInterfaceImageComponent::Spawn()
{
	Base::Spawn();

	Renderer.InterfaceRenderer().RegisterProxy(imageTextureProxy_);
	CreateImageTextureProxy();

	EventRectUpdated().AddListener(KtDelegate(this, &KInterfaceImageComponent::MarkImageTextureProxyRectDirty));
	GetEventShaderUpdated().AddListener(KtDelegate(this, &KInterfaceImageComponent::MarkImageTextureProxyShaderDirty));
	GetEventImageTextureUpdated().AddListener(KtDelegate(this, &KInterfaceImageComponent::MarkImageTextureProxyImageTextureDirty));
	EventLayerUpdated().AddListener(KtDelegate(this, &KInterfaceImageComponent::MarkImageTextureProxyLayerDirty));
}

void KInterfaceImageComponent::CreateImageTextureProxy()
{
	imageTextureProxy_->ScheduleUpdate(
		[this](UInterfaceProxy::FrameData& frameData)
		{
			frameData.shader = GetShader();
			frameData.renderable = GetImageTexture();;
			frameData.layer = GetLayer();
			frameData.objectData.modelMatrix = ModelMatrix();
			frameData.objectData.color = GetColor();
			frameData.scissor.offset = WindowViewport.GetOffset();
			frameData.scissor.extent = WindowViewport.GetExtent();
		}
	);
}

void KInterfaceImageComponent::MarkImageTextureProxyRectDirty()
{
	imageTextureProxy_->ScheduleUpdate(
		[this](UInterfaceProxy::FrameData& frameData)
		{
			frameData.objectData.modelMatrix = ModelMatrix();
		}
	);
}

void KInterfaceImageComponent::MarkImageTextureProxyShaderDirty()
{
	imageTextureProxy_->ScheduleUpdate(
		[this](UInterfaceProxy::FrameData& frameData)
		{
			frameData.shader = GetShader();
		}
	);
}

void KInterfaceImageComponent::MarkImageTextureProxyImageTextureDirty()
{
	imageTextureProxy_->ScheduleUpdate(
		[this](UInterfaceProxy::FrameData& frameData)
		{
			frameData.renderable = GetImageTexture();
		}
	);
}

void KInterfaceImageComponent::MarkImageTextureProxyLayerDirty()
{
	imageTextureProxy_->ScheduleUpdate(
		[this](UInterfaceProxy::FrameData& frameData)
		{
			frameData.layer = GetLayer();
		}
	);
}
