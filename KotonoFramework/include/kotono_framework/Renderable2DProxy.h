#pragma once
#include "ObjectData2D.h"
#include "Scissor.h"
class KtShader;
class KtWindowViewport;
class KtRenderable2D;
struct KtRenderable2DProxy final
{
	KtShader* shader;
	KtRenderable2D* renderable;
	int32_t layer;
	KtObjectData2D objectData;
	KtScissor scissor;	

	bool isDirty;

#ifdef _DEBUG
	void* source;
#endif
};
