#pragma once
#include "ObjectData3D.h"
class KtShader;
class KtRenderable3D;
class KtViewport;
struct KtRenderable3DProxy final
{
	KtShader* shader;
	KtRenderable3D* renderable;
	KtViewport* viewport;
	KtObjectData3D objectData;

    bool isDirty;
};

