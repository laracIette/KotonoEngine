#pragma once
#include "frames_in_flight.h"
#include "InterfaceObjectData.h"
#include "Scissor.h"
#include <functional>
#include <kotono_common/Pool.h>
class KtShader;
class KtInterfaceRenderable;
class UInterfaceProxy final
{
private:
	friend class KtInterfaceCuller;
	friend class KtInterfaceRenderer;

public:
	struct Data
	{
		KtShader* shader;
		KtInterfaceRenderable* renderable;
		i32 layer;
		KtInterfaceObjectData objectData;
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
	KtPool<UpdateData> pendingUpdates_;
};
