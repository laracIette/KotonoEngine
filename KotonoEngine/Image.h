#pragma once
#include "InterfaceObject.h"

class KtShader;
class KtImageTexture;

class RImage : public RInterfaceObject
{
	BASECLASS(RInterfaceObject)

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
	KtShader* _shader;
	KtImageTexture* _imageTexture;

	void AddTextureToRenderQueue();
	void AddWireframeToRenderQueue();
};

