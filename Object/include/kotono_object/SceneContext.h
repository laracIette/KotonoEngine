#pragma once
#include "generated/SceneContext.generated.h"
#include <kotono_object/Widget.h>
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

	UScene* GetScene() const;

	void Deserialize() override;

private:
	SERIALIZE UPath scenePath_;
	UScene* scene_;
};