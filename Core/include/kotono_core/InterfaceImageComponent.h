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
	KInterfaceImageComponent(UPtrOwnerBase* ptrOwner);
	~KInterfaceImageComponent() override;

protected:
	void Init() override;

public:
	KtShader* GetShader() const;
	KtTexture* GetTexture() const;

	UEvent<>& GetEventShaderUpdated();
	UEvent<>& GetEventTextureUpdated();

	void SetShader(KtShader* shader);
	void SetTexture(KtTexture* texture);

	void Spawn() override;

private:
	void CreateTextureProxy();
	void MarkTextureProxyRectDirty();
	void MarkTextureProxyShaderDirty();
	void MarkTextureProxyTextureDirty();
	void MarkTextureProxyLayerDirty();

private:
	KtShader* shader_;
	KtTexture* texture_;
	UInterfaceProxy* textureProxy_;
	UEvent<> eventShaderUpdated_;
	UEvent<> eventTextureUpdated_;
};

