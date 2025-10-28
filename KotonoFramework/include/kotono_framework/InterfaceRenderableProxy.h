#pragma once
#include "ObjectData2D.h"
#include "Scissor.h"
class KtShader;
class KtWindowViewport;
class KtInterfaceRenderable;
struct KtInterfaceRenderableProxy final
{
	KtShader* shader;
	KtInterfaceRenderable* renderable;
	int32_t layer;
	KtObjectData2D objectData;
	KtScissor scissor;	

	bool isDirty;

#	ifdef _DEBUG
		void* source;
#	endif
};
