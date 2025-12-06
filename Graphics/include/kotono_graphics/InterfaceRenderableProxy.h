#pragma once
#include "InterfaceObjectData.h"
#include "Scissor.h"
class KtShader;
class KtWindowViewport;
class KtInterfaceRenderable;
struct KtInterfaceRenderableProxy final
{
	KtShader* shader;
	KtInterfaceRenderable* renderable;
	int32_t layer;
	KtInterfaceObjectData objectData;
	KtScissor scissor;	

	bool isDirty;

#	ifdef _DEBUG
		void* source;
#	endif
};
