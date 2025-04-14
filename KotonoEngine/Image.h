#pragma once
#include "InterfaceObject.h"
#include "InterfaceCollider.h"
#include <kotono_framework/Shader.h>
class RImage : public RInterfaceObject
{
	BASECLASS(RInterfaceObject)

public:
	void Init() override;
	void Update() override;
	void Cleanup() override;

	KtShader* GetShader() const;
	void SetShader(KtShader* shader);

private:
	KtShader* _shader;
	RInterfaceCollider* _collider;

	void Draw();
	void AddToRenderQueue() const;

	// temp
	void OnEventOverlap(RInterfaceCollider* other);
	void OnEventMouseLeftButtonDown();
};

