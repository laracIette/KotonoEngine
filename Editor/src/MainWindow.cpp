#include "MainWindow.h"

#include "AssetExplorer.h"
#include "DefaultSceneContext.h"
#include "Detachable.h"
#include "PropertiesWindow.h"
#include "UpdateTimeText.h"
#include <kotono_core/ProjectSettings.h>
#include <kotono_interface/widgets.h>

WidgetPtr WMainWindow::Build()
{
	auto const sceneContext{ UCreate<WDefaultSceneContext>{ "Scene Context" }(SProjectSettings::Get<std::string>("/startupScene")) };
	AddSceneContext(sceneContext);

	const auto widgetTree{ UChildrenOwnerTree{ UCreate<WColumn>{ "Main Window Column" }(5.0f), {
		new UChildrenOwnerTree{ UCreate<WRow>{ "Top Row" }(), {
			new UWidgetTreeLeaf{ UCreate<WSpacer>{ "Top Row Spacer" }(EAxis::Horizontal) },
			new UChildOwnerTree{ UCreate<WWrap>{ "Times Wrap" }(),
				new UWidgetTreeLeaf{ UCreate<WUpdateTimeText>{ "Update Time Text" }() },
			},
		} },
		
		new UChildrenOwnerTree{ UCreate<WRow>{ "Center Row" }(10.0f), {
			new UChildrenOwnerTree{ UCreate<WColumn>{ "Left Panel Column" }(10.0f), {
				new UWidgetTreeLeaf{ sceneContext },
				new UChildOwnerTree{ UCreate<WConstraint>{ "Asset Explorer Constraint" }(EAxis::Vertical, 325.0f),
					new UChildOwnerTree{ UCreate<WDetachable>{ "Asset Explorer" }(),
						new UWidgetTreeLeaf{ UCreate<WAssetExplorer>{ "Asset Explorer" }() }
					}
				},
			} },

			new UChildOwnerTree{ UCreate<WWrap>{ "Right Panel Wrap" }(EAxis::Horizontal),
				new UChildrenOwnerTree{ UCreate<WStack>{ "Right Panel Stack" }(), {
					new UWidgetTreeLeaf{ UCreate<WColor>{ "Right Panel Background" }(Colors::White.WithValue(0.5f).WithAlpha(0.4f))},
					new UChildOwnerTree{ UCreate<WPadding>{ "Right Panel Padding" }(UPadding::All(8.0f)),
						new UChildrenOwnerTree{ UCreate<WColumn>{ "Right Panel Column" }(4.0f), {
							new UWidgetTreeLeaf{ UCreate<WPropertiesWindow>{ "Properties Window" }() },
						} }
					},
				} }
			},
		} },
	} } };

	widgetTree.Link();

	return widgetTree.Widget();
}

#include "generated/MainWindow.generated.inl"
