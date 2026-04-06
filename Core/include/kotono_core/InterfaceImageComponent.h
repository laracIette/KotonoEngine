#pragma once
#include "generated/InterfaceImageComponent.generated.h"
#include "InterfaceComponent.h"

class KtShader;
class KtTexture;
class UInterfaceProxy;

class KInterfaceImageComponent : public KInterfaceComponent
{
	GENERATED_KINTERFACEIMAGECOMPONENT()

public:
	KInterfaceImageComponent(UPtrOwner* ptrOwner);
	~KInterfaceImageComponent() override;

protected:
	void Init() override;

public:
	UAsset<KtShader> GetShader() const;
	UAsset<KtTexture> GetTexture() const;

	UEvent<>& GetEventShaderUpdated();
	UEvent<>& GetEventTextureUpdated();

	void SetShader(UAsset<KtShader> shader);
	void SetTexture(UAsset<KtTexture> texture);

	void Spawn() override;

private:
	void CreateTextureProxy();
	void MarkTextureProxyRectDirty();
	void MarkTextureProxyShaderDirty();
	void MarkTextureProxyTextureDirty();
	void MarkTextureProxyLayerDirty();

private:
	UAsset<KtShader> shader_;
	UAsset<KtTexture> texture_;
	UInterfaceProxy* textureProxy_;
	UEvent<> eventShaderUpdated_;
	UEvent<> eventTextureUpdated_;
};

