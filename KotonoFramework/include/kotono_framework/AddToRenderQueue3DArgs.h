#pragma once
#include "ObjectData3D.h"

class KtShader;
class KtRenderable3D;
class KtViewport;

struct KtAddToRenderQueue3DArgs
{
	KtShader* Shader;
	KtRenderable3D* Renderable;
	KtViewport* Viewport;
	KtObjectData3D ObjectData;
};

