#pragma once
#include "ObjectData3D.h"
#include "Scissor.h"
class KtShader;
class KtRenderable3D;
class KtWindowViewport;
struct KtRenderable3DProxy final
{
	KtShader* shader;
	KtRenderable3D* renderable;
	KtObjectData3D objectData;
	KtScissor scissor;

    bool isDirty;

#	ifdef _DEBUG
		void* source;
#	endif
};

