#pragma once
#include "Viewport.h"
#include "Shader2D.h"
#include "ObjectData2D.h"
struct KtAddToRenderQueue2DArgs
{
	KtShader2D* Shader;
	KtViewport* Viewport;
	KtObjectData2D ObjectData;
};

