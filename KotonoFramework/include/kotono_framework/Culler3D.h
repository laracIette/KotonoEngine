#pragma once
#include <unordered_set>
#include "Culler3DField.h"
struct KtRenderable3DProxy;
class KtCuller3D final
{
private:
	using ProxiesUnorderedSet = std::unordered_set<KtRenderable3DProxy*>;

public:
	const ProxiesUnorderedSet ComputeCulling(ProxiesUnorderedSet renderQueueData, const KtCuller3DField field) const;

private:
	const ProxiesUnorderedSet ComputeNullCulling(const ProxiesUnorderedSet& renderQueueData) const;
	const ProxiesUnorderedSet ComputeDistanceCulling(const ProxiesUnorderedSet& renderQueueData) const;
};

