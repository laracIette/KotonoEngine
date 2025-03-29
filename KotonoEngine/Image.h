#pragma once
#include "InterfaceObject.h"
//#include <kotono_framework/Shader.h>
#include <kotono_framework/ImageTexture.h>
class RImage : public RInterfaceObject
{
	BASECLASS(RInterfaceObject)

public:
private:
	//KtShader* _shader;
	KtImageTexture* _imageTexture;
};

