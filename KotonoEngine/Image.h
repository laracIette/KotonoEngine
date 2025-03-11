#pragma once
#include "InterfaceObject.h"
//#include <kotono_framework/Shader2D.h>
#include <kotono_framework/ImageTexture.h>
class RImage : public RInterfaceObject
{
	BASECLASS(RInterfaceObject)

public:
private:
	//KtShader2D* _shader;
	KtImageTexture* _imageTexture;
};

