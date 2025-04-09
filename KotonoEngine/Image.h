#pragma once
#include "InterfaceObject.h"
#include <kotono_framework/Shader.h>
#include "Task.h"
class RImage : public RInterfaceObject
{
	BASECLASS(RInterfaceObject)

public:
	void Update() override;

	KtShader* GetShader() const;
	void SetShader(KtShader* shader);

	void AddToRenderQueue() const;

private:
	KtShader* _shader;
};

