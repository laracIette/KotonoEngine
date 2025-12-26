#include "InterfaceProxy.h"

void UInterfaceProxy::ScheduleUpdate(const UpdateFunction& function)
{
	pendingUpdates_.Add({ function, static_cast<uint32_t>(KT_FRAMES_IN_FLIGHT) });
}

void UInterfaceProxy::ApplyPendingUpdates(const uint32_t frameIndex)
{
	if (pendingUpdates_.Empty())
	{
		return;
	}

	for (auto& updateData : pendingUpdates_)
	{
		updateData.function(frameDatas_[frameIndex]);
		--updateData.count;
	}

	pendingUpdates_.RemoveIf(
		[](const UpdateData& updateData)
		{
			return updateData.count == 0;
		}
	);
}

bool UInterfaceProxy::IsDirty() const
{
	return !pendingUpdates_.Empty();
}
