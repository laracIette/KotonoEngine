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
	UPtr<WDefaultSceneContext> sceneContext;

	const auto widgetTree{ UChildrenOwnerTree{ UCreate<WColumn>{ "Main Window Column" }()
		| Apply(&WColumn::SetSpacing, 5.0f), {
		
		new UChildrenOwnerTree{ UCreate<WRow>{ "Top Row" }(), {

			new UWidgetTreeLeaf{ UCreate<WSpacer>{ "Top Row Spacer" }(EAxis::Horizontal) },

			new UChildOwnerTree{ UCreate<WWrap>{ "Times Wrap" }(),

				new UWidgetTreeLeaf{ UCreate<WUpdateTimeText>{ "Update Time Text" }() },

			},
		} },
		
		new UChildrenOwnerTree{ UCreate<WRow>{ "Center Row" }()
			| Apply(&WRow::SetSpacing, 10.0f), {

			new UChildrenOwnerTree{ UCreate<WColumn>{ "Left Panel Column" }()
				| Apply(&WColumn::SetSpacing, 10.0f), {

				new UWidgetTreeLeaf{ sceneContext = UCreate<WDefaultSceneContext>{ "Scene Context" }(SProjectSettings::Get<std::string>("/startupScene")) },

				new UChildOwnerTree{ UCreate<WConstraint>{ "Asset Explorer Constraint" }()
					| Apply(&WConstraint::SetAxis, EAxis::Vertical)
					| Apply(&WConstraint::SetSize, 325.0f),

					new UChildOwnerTree{ UCreate<WDetachable>{ "Asset Explorer" }(),

						new UWidgetTreeLeaf{ UCreate<WAssetExplorer>{ "Asset Explorer" }() }

					}
				},
			} },

			// todo, needs scene ptr (scene widget)
			//new UWidgetTreeLeaf{ UCreate<WPropertiesWindow>{ "Properties Window" }(GetScene()) },

		} },

	} } };

	AddSceneContext(sceneContext);

	widgetTree.Link();

	return widgetTree.Widget();
}

#include "generated/MainWindow.generated.inl"
