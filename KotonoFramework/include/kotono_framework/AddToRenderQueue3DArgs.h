#pragma once
#include "Shader3D.h"
#include "Model.h"
#include "Viewport.h"
#include "ObjectData3D.h"
struct KtAddToRenderQueue3DArgs
{
	KtShader3D* Shader;
	KtModel* Model;
	KtViewport* Viewport;
	KtObjectData3D ObjectData;
};

