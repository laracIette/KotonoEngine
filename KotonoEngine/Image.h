#pragma once
#include "InterfaceObject.h"
#include <kotono_framework/Shader.h>
class RImage : public RInterfaceObject
{
	BASECLASS(RInterfaceObject)

public:
	KtShader* GetShader() const;
	void SetShader(KtShader* shader);

	void AddToRenderQueue() const;

private:
	KtShader* _shader;
};

