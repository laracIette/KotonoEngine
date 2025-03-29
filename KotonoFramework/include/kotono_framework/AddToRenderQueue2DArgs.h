#pragma once
#include "Viewport.h"
#include "Shader.h"
#include "ObjectData2D.h"
struct KtAddToRenderQueue2DArgs
{
	KtShader* Shader;
	KtViewport* Viewport;
	KtObjectData2D ObjectData;
};

