#pragma once
#include "ObjectData3D.h"
class KtShader;
class KtRenderable3D;
class KtWindowViewport;
struct KtRenderable3DProxy final
{
	KtShader* shader;
	KtRenderable3D* renderable;
	KtWindowViewport* viewport;
	KtObjectData3D objectData;

    bool isDirty;
};

