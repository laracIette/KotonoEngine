#pragma once
#include "InterfaceComponent.h"

class KtShader;
class KtImageTexture;

class KInterfaceImageComponent : public KInterfaceComponent
{
	BASECLASS(KInterfaceComponent)

public:
	void Construct() override;
	void Init() override;
	void Update() override;
	void Cleanup() override;

	KtShader* GetShader() const;
	KtImageTexture* GetImageTexture() const;

	void SetShader(KtShader* shader);
	void SetImageTexture(KtImageTexture* imageTexture);

private:
	KtShader* shader_;
	KtImageTexture* _imageTexture;

	void AddTextureToRenderQueue();
	void AddWireframeToRenderQueue();
};

