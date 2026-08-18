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
class UInterface final
{
public:
	UInterface();

	EHandle GetTextureHandle(UPath const& path);
	std::span<UPendingTexture const> GetPendingTextures() const;
	void ClearPendingTextures();

	EHandle RegisterRenderTarget(glm::uvec2 const& extent);
	void UnregisterRenderTarget(EHandle handle);
	void SetRenderTargetData(EHandle handle, USceneView const& sceneView);
	std::unordered_map<EHandle, USceneView> const& GetSceneViews();

	std::span<UPendingSceneRender const> GetPendingSceneRenders() const;
	std::unordered_multimap<glm::uvec2, EHandle> const& GetUnusedSceneRenders() const;
	void ClearPendingSceneRenders();
	void ClearUnusedSceneRenders();

	void Update(f32 deltaTime);
	UTimeContext const& GetTimeContext() const;

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
};