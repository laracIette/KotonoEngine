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
		uint32_t count;
	};

	struct PoolData
	{
		bool isRegistered;
		size_t index;
	};

	struct FrameData
	{
		Data data;
		PoolData poolData;
	};

public:
	void ScheduleUpdate(const UpdateFunction& function);

private:
	void ApplyPendingUpdates(const uint32_t frameIndex);

	bool IsDirty() const;

private:
	KtFramesInFlightArray<FrameData> frameDatas_;
	KtPool<UpdateData> pendingUpdates_;
};
