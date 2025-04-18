#pragma once
#include "InterfaceObject.h"
#include "InterfaceCollider.h"
#include <kotono_framework/Shader.h>
#include <kotono_framework/ImageTexture.h>
class RImage : public RInterfaceObject
{
	BASECLASS(RInterfaceObject)

public:
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
	RInterfaceCollider* _collider;

	void Draw();
	void AddToRenderQueue() const;

	// temp
	void OnEventOverlap(RInterfaceCollider* other);
	void OnEventMouseLeftButtonDown();
};

