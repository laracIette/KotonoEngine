#pragma once
#include "InterfaceComponent.h"
#include <kotono_framework/Renderable2DProxy.h>

class KtShader;
class KtImageTexture;

class KInterfaceImageComponent : public KInterfaceComponent
{
	BASECLASS(KInterfaceComponent)

public:
	KInterfaceImageComponent(RInterfaceObject* owner);

protected:
	void Init() override;
	void Update() override;
	void Cleanup() override;

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
	KtRenderable2DProxy imageTextureProxy_;
	KtEvent<> eventShaderUpdated_;
	KtEvent<> eventImageTextureUpdated_;

	void CreateImageTextureProxy();
	void MarkImageTextureProxyRectDirty();
	void MarkImageTextureProxyShaderDirty();
	void MarkImageTextureProxyImageTextureDirty();
	void MarkImageTextureProxyLayerDirty();
};

