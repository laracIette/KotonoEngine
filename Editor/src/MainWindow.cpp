#include "MainWindow.h"

#include "AssetExplorer.h"
#include "DefaultSceneContext.h"
#include "Detachable.h"
#include "UpdateTimeText.h"
#include <kotono_core/ProjectSettings.h>
#include <kotono_interface/widgets.h>

WidgetPtr WMainWindow::Build()
{
	UPtr const sceneContext{ UCreate<WDefaultSceneContext>{ "Scene Context" }(SProjectSettings::Get<std::string>("/startupScene")) };
	AddSceneContext(sceneContext);
	
	return (
		UCreate<WColumn>{ "Main Window Column" }()
		| Apply(&WColumn::SetSpacing, 5.0f)
		| (
			UCreate<WWrap>{}()
			| Apply(&WWrap::SetAxis, EAxis::Vertical)
			| (
				UCreate<WAlign>{}()
				| Apply(&WAlign::SetAlignment, UAlignment::Right())
				| (
					UCreate<WUpdateTimeText>{ "Update Time Text" }()
				)
			)
		)
		| (
			UCreate<WRow>{}()
			| (
				sceneContext
			)
		)
		| (
			UCreate<WConstraint>{ "Asset Explorer Constraint" }()
			| Apply(&WConstraint::SetAxis, EAxis::Vertical)
			| Apply(&WConstraint::SetSize, 250.0f)
			| (
				UCreate<WDetachable>{ "Asset Explorer" }()
				| (
					UCreate<WAssetExplorer>{ "Asset Explorer" }()
				)
			)
		)
	);
}

#include "generated/MainWindow.generated.inl"
