#pragma once
#include "SceneObjectData.h"
#include "Scissor.h"
#include <atomic>
#include <kotono_graphics/frames_in_flight.h>
class KtShader;
class KtSceneRenderable;
struct KtSceneRenderableProxy final
{
	KtShader* shader;
	KtSceneRenderable* renderable;
	KtSceneObjectData objectData;
	KtScissor scissor;

    KtFramesInFlightArray<std::atomic<bool>> isDirty;

	void MarkDirty()
	{
		for (auto& dirty : isDirty)
		{
			dirty.store(true, std::memory_order_release);
		}
	}

#	ifdef _DEBUG
		void* source;
#	endif
};

