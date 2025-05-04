#pragma once
#include "Shader.h"
#include "Renderable2D.h"
#include "Viewport.h"
#include "ObjectData2D.h"
struct KtAddToRenderQueue2DArgs
{
	KtShader* Shader;
	KtRenderable2D* Renderable;
	KtViewport* Viewport;
	KtObjectData2D ObjectData;
	int32_t Layer;
};

