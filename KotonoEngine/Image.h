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

	void AddToRenderQueue() const;

private:
	KtShader* _shader;
	RInterfaceCollider* _collider;



	// temp
	void OnEventOverlap(RInterfaceCollider* other);
	void OnEventMouseLeftButtonDown();
};

