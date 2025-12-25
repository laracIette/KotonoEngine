#pragma once
#include "generated/InterfaceImageComponent.generated.h"
#include "InterfaceComponent.h"

class KtShader;
class KtImageTexture;
struct KtInterfaceRenderableProxy;

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

private:
	KtShader* shader_;
	KtImageTexture* imageTexture_;
	KtInterfaceRenderableProxy* imageTextureProxy_;
	KtEvent<> eventShaderUpdated_;
	KtEvent<> eventImageTextureUpdated_;

	void CreateImageTextureProxy();
	void MarkImageTextureProxyRectDirty();
	void MarkImageTextureProxyShaderDirty();
	void MarkImageTextureProxyImageTextureDirty();
	void MarkImageTextureProxyLayerDirty();
};

