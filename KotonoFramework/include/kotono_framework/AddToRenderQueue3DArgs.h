#pragma once
#include "Viewport.h"
#include "Shader3D.h"
#include "Model.h"
#include "ObjectData3D.h"
struct KtAddToRenderQueue3DArgs
{
	KtViewport Viewport;
	KtShader3D* Shader;
	KtModel* Model;
	KtObjectData3D ObjectData;
};

