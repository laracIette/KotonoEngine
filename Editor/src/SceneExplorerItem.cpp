#include "SceneExplorerItem.h"
#include <kotono_core/ObjectManager.h>
#include <kotono_core/SceneObject.h>
#include <kotono_interface/widgets.h>

WSceneExplorerItem::WSceneExplorerItem(const UPtr<TSceneObject>& sceneObject)
	: sceneObject_(sceneObject)
{
}

WWidget* WSceneExplorerItem::Build()
{
	ObjectManager.EventSelectedObjectChanged().AddListener(UDelegate(this, &WSceneExplorerItem::Refresh));

	return new WWrap({
		.child = new WStack({
			.children = {
				sceneObject_ && (ObjectManager.GetSelectedObject() == sceneObject_)
					? new WColor({ UColor::Black().WithAlpha(0.2f) })
					: new WColor({ UColor::Transparent() }),
				new WButton({
					.onPress = [this]() {
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
	ObjectManager.EventSelectedObjectChanged().RemoveListener(UDelegate(this, &WSceneExplorerItem::Refresh));

	WWidget::Cleanup();
}