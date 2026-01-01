#include "SceneProxy.h"

void USceneProxy::ScheduleUpdate(const UpdateFunction& function)
{
	pendingUpdates_.Add({ function, static_cast<u32>(KT_FRAMES_IN_FLIGHT) });
}

void USceneProxy::ApplyPendingUpdates(const u32 frameIndex)
{
	if (pendingUpdates_.empty())
	{
		return;
	}

	for (auto& updateData : pendingUpdates_)
	{
		updateData.function(frameDatas_[frameIndex].data);
		--updateData.count;
	}

	pendingUpdates_.RemoveIf(
		[](const UpdateData& updateData)
		{
			return updateData.count == 0;
		}
	);
}

bool USceneProxy::IsDirty() const
{
	return !pendingUpdates_.empty();
}
