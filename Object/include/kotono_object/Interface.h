#pragma once
#include "Ptr.h"
#include <glm/ext/vector_uint2.hpp>
#include <glm/gtx/hash.hpp>
#include <kotono_common/Handle.h>
#include <kotono_common/Path.h>
#include <kotono_graphics/SceneView.h>
#include <kotono_timing/TimeContext.h>
#include <span>
#include <unordered_map>
#include <vector>
struct UPendingTexture;
struct UPendingSceneRender;
struct UInterfaceRenderGraph;
struct USceneRenderGraph;
class WSceneContext;
class UInterface final
{
public:
	UInterface();
	~UInterface();

	EHandle GetTextureHandle(UPath const& path);
	void ClearPendingTextures();

	EHandle RegisterRenderTarget(glm::uvec2 const& extent);
	void UnregisterRenderTarget(EHandle handle);
	void SetRenderTargetData(EHandle handle, USceneView const& sceneView);

	void ClearPendingSceneRenders();
	void ClearUnusedSceneRenders();

	void PopulateInterfaceRenderGraph(UInterfaceRenderGraph& interfaceRenderGraph) const;
	void PopulateSceneRenderGraph(USceneRenderGraph& sceneRenderGraph) const;

	void Update(f32 deltaTime);

	void BeginDraw(glm::uvec2 const& bounds);
	void EndDraw() const;

	std::span<UPendingTexture const> GetPendingTextures() const;
	std::span<UPendingSceneRender const> GetPendingSceneRenders() const;

	std::unordered_map<EHandle, USceneView> const&		GetSceneViews() const { return sceneViews_; }
	std::unordered_multimap<glm::uvec2, EHandle> const& GetUnusedSceneRenders() const { return unusedSceneRenders_; }

	UTimeContext const&	GetTimeContext() const { return timeContext_; }
	glm::uvec2 const&	GetBounds() const { return bounds_; }

	void SetWidget(UPtr<WSceneContext> const& widget) { widget_ = widget; }

private:
	EHandle currentHandle_;
	std::vector<EHandle> freeHandles_;

	std::unordered_map<UPath, EHandle> textures_;
	std::vector<UPendingTexture> pendingTextures_;

	std::unordered_map<EHandle, glm::uvec2> sceneRenders_;
	std::unordered_map<EHandle, USceneView> sceneViews_;

	std::vector<UPendingSceneRender> pendingSceneRenders_;
	std::unordered_multimap<glm::uvec2, EHandle> unusedSceneRenders_;

	UTimeContext timeContext_;

	UPtr<WSceneContext> widget_;
	glm::uvec2 bounds_;
};