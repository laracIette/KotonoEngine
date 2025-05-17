#pragma once
#include "ObjectData2D.h"

class KtShader;
class KtRenderable2D;
class KtViewport;

struct KtAddToRenderQueue2DArgs
{
	KtShader* Shader;
	KtRenderable2D* Renderable;
	KtViewport* Viewport;
	KtObjectData2D ObjectData;
	int32_t Layer;
};

