#include "InterfaceImageComponent.h"
#include <kotono_graphics/ShaderManager.h>
#include <kotono_graphics/Renderer.h>
#include <kotono_graphics/Shader.h>
#include <kotono_graphics/Texture.h>
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
		WireframeShader = ShaderManager.Get("${ENGINE_DIRECTORY}/Graphics/shaders/wireframe2D.ktshader");
	}

	textureProxy_ = Renderer.InterfaceRenderer().CreateProxy();
}

void KInterfaceImageComponent::Cleanup()
{
	Base::Cleanup();

	Renderer.InterfaceRenderer().UnregisterProxy(textureProxy_);
	Renderer.InterfaceRenderer().DeleteProxy(textureProxy_);
}

void KInterfaceImageComponent::Init()
{
	Base::Init();
}

KtShader* KInterfaceImageComponent::GetShader() const
{
	return shader_;
}

KtTexture* KInterfaceImageComponent::GetTexture() const
{
	return texture_;
}

KtEvent<>& KInterfaceImageComponent::GetEventShaderUpdated()
{
	return eventShaderUpdated_;
}

KtEvent<>& KInterfaceImageComponent::GetEventTextureUpdated()
{
	return eventTextureUpdated_;
}

void KInterfaceImageComponent::SetShader(KtShader* shader)
{
	shader_ = shader;
	eventShaderUpdated_.Broadcast();
}

void KInterfaceImageComponent::SetTexture(KtTexture* texture)
{
	texture_ = texture;
	eventTextureUpdated_.Broadcast();
}

void KInterfaceImageComponent::Spawn()
{
	Base::Spawn();

	CreateTextureProxy();
	Renderer.InterfaceRenderer().RegisterProxy(textureProxy_);

	EventRectChanged().AddListener(KtDelegate(this, &KInterfaceImageComponent::MarkTextureProxyRectDirty));
	GetEventShaderUpdated().AddListener(KtDelegate(this, &KInterfaceImageComponent::MarkTextureProxyShaderDirty));
	GetEventTextureUpdated().AddListener(KtDelegate(this, &KInterfaceImageComponent::MarkTextureProxyTextureDirty));
	EventLayerChanged().AddListener(KtDelegate(this, &KInterfaceImageComponent::MarkTextureProxyLayerDirty));
}

void KInterfaceImageComponent::CreateTextureProxy()
{
	textureProxy_->ScheduleUpdate(
		[this](UInterfaceProxy::Data& data)
		{
			data.shader = GetShader();
			data.renderable = GetTexture();;
			data.layer = GetLayer();
			data.objectData.modelMatrix = ModelMatrix();
			data.objectData.color = GetColor();
			data.scissor.offset = WindowViewport.GetOffset();
			data.scissor.extent = WindowViewport.GetExtent();
		}
	);
}

void KInterfaceImageComponent::MarkTextureProxyRectDirty()
{
	textureProxy_->ScheduleUpdate(
		[this](UInterfaceProxy::Data& data)
		{
			data.objectData.modelMatrix = ModelMatrix();
		}
	);
}

void KInterfaceImageComponent::MarkTextureProxyShaderDirty()
{
	textureProxy_->ScheduleUpdate(
		[this](UInterfaceProxy::Data& data)
		{
			data.shader = GetShader();
		}
	);
}

void KInterfaceImageComponent::MarkTextureProxyTextureDirty()
{
	textureProxy_->ScheduleUpdate(
		[this](UInterfaceProxy::Data& data)
		{
			data.renderable = GetTexture();
		}
	);
}

void KInterfaceImageComponent::MarkTextureProxyLayerDirty()
{
	textureProxy_->ScheduleUpdate(
		[this](UInterfaceProxy::Data& data)
		{
			data.layer = GetLayer();
		}
	);
}
