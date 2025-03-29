#pragma once
#include "Shader.h"
#include "Model.h"
#include "Viewport.h"
#include "ObjectData3D.h"
struct KtAddToRenderQueue3DArgs
{
	KtShader* Shader;
	KtModel* Model;
	KtViewport* Viewport;
	KtObjectData3D ObjectData;
};

