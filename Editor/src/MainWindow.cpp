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

	UPtr widget = UCreate<WColumn>{ "Main Window Column" }()
	| Apply(&WColumn::SetSpacing, 5.0f)
	| (
		UCreate<WRow>{ "Top Row" }()
		| (
			UCreate<WSpacer>{ "Top Row Spacer" }(EAxis::Horizontal)
		)
		| (
			UCreate<WWrap>{ "Times Wrap" }()
			| (

				UCreate<WUpdateTimeText>{ "Update Time Text" }()
			)
		)
	)
	| (
		UCreate<WRow>{ "Center Row" }()
		| Apply(&WRow::SetSpacing, 10.0f)
		| (
			UCreate<WColumn>{ "Left Panel Column" }()
			| Apply(&WColumn::SetSpacing, 10.0f)
			| (
				sceneContext = UCreate<WDefaultSceneContext>{ "Scene Context" }(SProjectSettings::Get<std::string>("/startupScene"))
			)
			| (
				UCreate<WConstraint>{ "Asset Explorer Constraint" }()
				| Apply(&WConstraint::SetAxis, EAxis::Vertical)
				| Apply(&WConstraint::SetSize, 325.0f)
				| (
					UCreate<WDetachable>{ "Asset Explorer" }()
					| (
						UCreate<WAssetExplorer>{ "Asset Explorer" }()
					)
				)
			)
		)
		// todo: needs scene ptr (scene widget)
		//| (
		//	UCreate<WPropertiesWindow>{ "Properties Window" }(GetScene())
		//)
	)
	;

	AddSceneContext(sceneContext);

	return widget;
}

#include "generated/MainWindow.generated.inl"
