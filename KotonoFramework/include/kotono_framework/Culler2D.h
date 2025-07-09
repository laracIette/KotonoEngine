#pragma once
#include <unordered_set>
struct KtRenderable2DProxy;
class KtCuller2D final
{
private:
	using ProxiesUnorderedSet = std::unordered_set<KtRenderable2DProxy*>;

public:
	const ProxiesUnorderedSet ComputeCulling(ProxiesUnorderedSet renderQueueData) const;

private:
	const ProxiesUnorderedSet ComputeNullCulling(const ProxiesUnorderedSet& renderQueueData) const;
	const ProxiesUnorderedSet ComputeScreenCulling(const ProxiesUnorderedSet& renderQueueData) const;
};

