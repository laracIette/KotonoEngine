#pragma once
#include "generated/InterfaceImageComponent.generated.h"
#include "InterfaceComponent.h"

class KtShader;
class KtImageTexture;
class UInterfaceProxy;

class KInterfaceImageComponent : public KInterfaceComponent
{
	GENERATED_KINTERFACEIMAGECOMPONENT()

public:
	KInterfaceImageComponent(UPtrOwnerBase* ptrOwner);

protected:
	void Cleanup() override;

	void Init() override;

public:
	KtShader* GetShader() const;
	KtImageTexture* GetImageTexture() const;

	KtEvent<>& GetEventShaderUpdated();
	KtEvent<>& GetEventImageTextureUpdated();

	void SetShader(KtShader* shader);
	void SetImageTexture(KtImageTexture* imageTexture);

	void Spawn() override;

private:
	void CreateImageTextureProxy();
	void MarkImageTextureProxyRectDirty();
	void MarkImageTextureProxyShaderDirty();
	void MarkImageTextureProxyImageTextureDirty();
	void MarkImageTextureProxyLayerDirty();

private:
	KtShader* shader_;
	KtImageTexture* imageTexture_;
	UInterfaceProxy* imageTextureProxy_;
	KtEvent<> eventShaderUpdated_;
	KtEvent<> eventImageTextureUpdated_;
};

