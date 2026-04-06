#include "InterfaceImageComponent.h"
#include "InterfaceObject.h"
#include <kotono_common/AssetManager.h>
#include <kotono_graphics/InterfaceProxy.h>
#include <kotono_graphics/Renderer.h>
#include <kotono_graphics/Shader.h>
#include <kotono_graphics/Texture.h>
#include <kotono_platform/WindowViewport.h>
#include <kotono_common/log.h>

static UAsset<KtShader> WireframeShader;

KInterfaceImageComponent::KInterfaceImageComponent(UPtrOwnerBase* ptrOwner) 
	: Base(ptrOwner)
{
	if (!WireframeShader)
	{
		WireframeShader = UAssetManager<KtShader>::Get("${ENGINE_DIRECTORY}/Graphics/shaders/wireframe2D.ktshader");
	}

	textureProxy_ = Renderer.InterfaceRenderer().CreateProxy();
}

KInterfaceImageComponent::~KInterfaceImageComponent()
{
	Renderer.InterfaceRenderer().UnregisterProxy(textureProxy_);
	Renderer.InterfaceRenderer().DeleteProxy(textureProxy_);
}

void KInterfaceImageComponent::Init()
{
	Base::Init();
}

UAsset<KtShader> KInterfaceImageComponent::GetShader() const
{
	return shader_;
}

UAsset<KtTexture> KInterfaceImageComponent::GetTexture() const
{
	return texture_;
}

UEvent<>& KInterfaceImageComponent::GetEventShaderUpdated()
{
	return eventShaderUpdated_;
}

UEvent<>& KInterfaceImageComponent::GetEventTextureUpdated()
{
	return eventTextureUpdated_;
}

void KInterfaceImageComponent::SetShader(UAsset<KtShader> shader)
{
	shader_ = shader;
	eventShaderUpdated_.Broadcast();
}

void KInterfaceImageComponent::SetTexture(UAsset<KtTexture> texture)
{
	texture_ = texture;
	eventTextureUpdated_.Broadcast();
}

void KInterfaceImageComponent::Spawn()
{
	Base::Spawn();

	CreateTextureProxy();
	Renderer.InterfaceRenderer().RegisterProxy(textureProxy_);

	EventRectChanged().AddListener(this, &KInterfaceImageComponent::MarkTextureProxyRectDirty);
	GetEventShaderUpdated().AddListener(this, &KInterfaceImageComponent::MarkTextureProxyShaderDirty);
	GetEventTextureUpdated().AddListener(this, &KInterfaceImageComponent::MarkTextureProxyTextureDirty);
	EventLayerChanged().AddListener(this, &KInterfaceImageComponent::MarkTextureProxyLayerDirty);
}

void KInterfaceImageComponent::CreateTextureProxy()
{
	textureProxy_->ScheduleUpdate(
		[this](UInterfaceProxy::Data& data)
		{
			data.shader = GetShader().Get();
			data.renderable = GetTexture().Get();
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
			data.shader = GetShader().Get();
		}
	);
}

void KInterfaceImageComponent::MarkTextureProxyTextureDirty()
{
	textureProxy_->ScheduleUpdate(
		[this](UInterfaceProxy::Data& data)
		{
			data.renderable = GetTexture().Get();
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

#include "generated/InterfaceImageComponent.generated.inl"
