#include "MainWindow.h"

#include "AssetExplorer.h"
#include "DefaultSceneContext.h"
#include "Detachable.h"
#include "UpdateTimeText.h"
#include <kotono_core/ProjectSettings.h>
#include <kotono_interface/widgets.h>

WidgetPtr WMainWindow::Build()
{
	UPtr<WDefaultSceneContext> sceneContext;

	UPtr widget{ (
		UCreate<WColumn>{ "Main Window Column" }()
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
			UCreate<WColumn>{}()
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
	) };

	AddSceneContext(sceneContext);

	return widget;
}

#include "generated/MainWindow.generated.inl"
