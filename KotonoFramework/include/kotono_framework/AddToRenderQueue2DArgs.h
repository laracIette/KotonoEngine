#pragma once
#include "Viewport.h"
#include "Shader2D.h"
#include "ObjectData2D.h"
struct KtAddToRenderQueue2DArgs
{
	KtViewport Viewport;
	KtShader2D* Shader;
	KtObjectData2D ObjectData;
};

