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
	struct FrameData
	{
		KtShader* shader;
		KtInterfaceRenderable* renderable;
		int32_t layer;
		KtInterfaceObjectData objectData;
		KtScissor scissor;
	};

	using UpdateFunction = std::function<void(FrameData&)>;

	struct UpdateData
	{
		UpdateFunction function;
		uint32_t count;
	};

private:
	struct PoolData
	{
		bool isRegistered;
		size_t index;
	};

public:
	void ScheduleUpdate(const UpdateFunction& function);

private:
	void ApplyPendingUpdates(const uint32_t frameIndex);

	bool IsDirty() const;

private:
	KtFramesInFlightArray<FrameData> frameDatas_;
	KtFramesInFlightArray<PoolData> poolDatas_;
	KtPool<UpdateData> pendingUpdates_;
};
