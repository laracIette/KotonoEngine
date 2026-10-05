#pragma once
#include "Widget/Widget.h"

#include "SceneContext.generated.h"

struct USceneRenderGraph;
class UScene;

/// <summary>
/// Base class for a widget managing a scene
/// </summary>
class WSceneContext : public WWidget
{
	GENERATED()

public:
	//WSceneContext() = delete; // register_ breaks if default constructor deleted
	WSceneContext(UPath const& scenePath);
	~WSceneContext() override;

	void Deserialize() override;

	void Update(f32 deltaTime) const;

private:
	SERIALIZE UPath scenePath_;
	ReadonlyProperty(UScene*, scene_, Scene, Value);
};