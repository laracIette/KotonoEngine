#pragma once
#include "ObjectData3D.h"
class KtShader;
class KtViewport;
class KtRenderable3D;
struct KtRenderable3DProxy final
{
public:
	KtShader* shader;
	KtRenderable3D* renderable;
	KtViewport* viewport;
	KtObjectData3D objectData;

    bool isDirty;
};

