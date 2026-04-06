#include "SceneExplorerItem.h"
#include <kotono_core/SceneObject.h>
#include <kotono_interface/widgets.h>
#include <kotono_object/ObjectManager.h>

WSceneExplorerItem::WSceneExplorerItem(const UPtr<TSceneObject>& sceneObject)
	: sceneObject_(sceneObject)
{
}

WidgetPtr WSceneExplorerItem::Build()
{
	ObjectManager.EventSelectedObjectChanged().AddListener(this, &WSceneExplorerItem::Refresh);

	return new WWrap({
		.child = new WStack({
			.children = {
				sceneObject_ && (ObjectManager.GetSelectedObject() == sceneObject_)
					? new WColor({ Colors::Black.WithAlpha(0.2f) })
					: new WColor({ Colors::Transparent }),
				new WButton({
					.onPressed = [this]() {
						SetState([this]() {
							ObjectManager.SetSelectedObject(sceneObject_);
						});
					},
				}),
				new WText({
					.text = sceneObject_ ? sceneObject_->GetName() : "",
					.fontSize = { 20.0f, 24.0f },
					.spacing = -6.0f,
				}),
			},
		}),
	});
}

void WSceneExplorerItem::Cleanup()
{
	ObjectManager.EventSelectedObjectChanged().RemoveListener(this, &WSceneExplorerItem::Refresh);

	WWidget::Cleanup();
}