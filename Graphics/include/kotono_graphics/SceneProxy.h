#pragma once
#include "frames_in_flight.h"
#include "SceneObjectData.h"
#include "Scissor.h"
#include <functional>
#include <kotono_common/Pool.h>
class KtShader;
class KtSceneRenderable;
class USceneProxy final
{
	friend class KtSceneCuller;
	friend class KtSceneRenderer;

public:
	struct FrameData
	{
		KtShader* shader;
		KtSceneRenderable* renderable;
		KtSceneObjectData objectData;
		KtScissor scissor;
	};

	using UpdateFunction = std::function<void(FrameData&)>;

	struct UpdateData
	{
		UpdateFunction function;
		uint32_t count;
	};

public:
	void ScheduleUpdate(const UpdateFunction& function);

	void ApplyPendingUpdates(const uint32_t frameIndex);

	bool IsDirty() const;

private:
	KtFramesInFlightArray<FrameData> frameDatas_;
	KtPool<UpdateData> pendingUpdates_;
	KtFramesInFlightArray<size_t> index_;
};
