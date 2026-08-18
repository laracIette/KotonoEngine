#pragma once
#include "generated/MainWindow.generated.h"
#include <kotono_object/SceneContext.h>
struct USceneRenderGraph;
class WMainWindow final : public WSceneContext
{
	GENERATED_WMAINWINDOW()

protected:
	WidgetPtr Build() override;

public:
	void BeginDraw(glm::uvec2 const& extent);
	void EndDraw();

	void Update(f32 deltaTime);

	void PopulateSceneRenderGraph(USceneRenderGraph& sceneRenderGraph) const;

	void SetInterface(UInterface* newInterface);
	UInterface* GetInterface() const override;

private:
	UInterface* interface_;
};
