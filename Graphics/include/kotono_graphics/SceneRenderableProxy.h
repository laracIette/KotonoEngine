#pragma once
#include "SceneObjectData.h"
#include "Scissor.h"
class KtShader;
class KtSceneRenderable;
class KtWindowViewport;
struct KtSceneRenderableProxy final
{
	KtShader* shader;
	KtSceneRenderable* renderable;
	KtSceneObjectData objectData;
	KtScissor scissor;

    bool isDirty;

#	ifdef _DEBUG
		void* source;
#	endif
};

