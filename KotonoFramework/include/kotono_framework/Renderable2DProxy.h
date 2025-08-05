#pragma once
#include "ObjectData2D.h"
class KtShader;
class KtViewport;
class KtRenderable2D;
struct KtRenderable2DProxy final
{
	KtShader* shader;
	KtRenderable2D* renderable;
	KtViewport* viewport;
	int32_t layer;
	KtObjectData2D objectData;

	bool isDirty;

#ifdef _DEBUG
	void* source;
#endif
};

