#include "InterfaceImageComponent.h"
#include <kotono_graphics/Shader.h>
#include <kotono_graphics/Texture.h>

KInterfaceImageComponent::KInterfaceImageComponent() 
{
}

KInterfaceImageComponent::~KInterfaceImageComponent()
{
}

void KInterfaceImageComponent::Init()
{
	Base::Init();
}

UAsset<UShader> KInterfaceImageComponent::GetShader() const
{
	return shader_;
}

UAsset<UTexture> KInterfaceImageComponent::GetTexture() const
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

void KInterfaceImageComponent::SetShader(UAsset<UShader> shader)
{
	shader_ = shader;
	eventShaderUpdated_.Broadcast();
}

void KInterfaceImageComponent::SetTexture(UAsset<UTexture> texture)
{
	texture_ = texture;
	eventTextureUpdated_.Broadcast();
}

void KInterfaceImageComponent::Spawn()
{
	Base::Spawn();
}

#include "generated/InterfaceImageComponent.generated.inl"
