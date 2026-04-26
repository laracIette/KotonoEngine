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
private:
	friend class KtSceneCuller;
	friend class KtSceneRenderer;

public:
	struct Data
	{
		KtShader* shader;
		KtSceneRenderable* renderable;
		KtSceneObjectData objectData;
		KtScissor scissor;
	};

	using UpdateFunction = std::function<void(Data&)>;

private:
	struct UpdateData
	{
		UpdateFunction function;
		u32 count;
	};

	struct PoolData
	{
		bool isRegistered;
		size index;
	};

	struct FrameData
	{
		Data data;
		PoolData poolData;
	};

public:
	void ScheduleUpdate(const UpdateFunction& function);

private:
	void ApplyPendingUpdates(const u32 frameIndex);

	bool IsDirty() const;

private:
	KtFramesInFlightArray<FrameData> frameDatas_;
	UPool<UpdateData> pendingUpdates_;
};
