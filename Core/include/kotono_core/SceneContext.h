#pragma once
#include "generated/SceneContext.generated.h"
#include <kotono_core/Widget.h>
struct USceneRenderGraph;
class UScene;
/// <summary>
/// Base class for a widget managing a scene
/// </summary>
class WSceneContext : public WWidget
{
	GENERATED_WSCENECONTEXT()

public:
	WSceneContext(UPath const& scenePath);
	~WSceneContext() override;

	void Deserialize() override;

	void Update(f32 deltaTime) const;

	void PopulateSceneRenderGraph(USceneRenderGraph& sceneRenderGraph) const;

	UScene* GetScene() const { return scene_; }

private:
	SERIALIZE UPath scenePath_;
	UScene* scene_;
};