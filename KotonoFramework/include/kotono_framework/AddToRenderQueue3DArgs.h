#pragma once
#include "Shader.h"
#include "Renderable3D.h"
#include "Viewport.h"
#include "ObjectData3D.h"
struct KtAddToRenderQueue3DArgs
{
	KtShader* Shader;
	KtRenderable3D* Renderable;
	KtViewport* Viewport;
	KtObjectData3D ObjectData;
};

