#include "MainWindow/MainWindow.h"

#include "AssetExplorer/AssetExplorer.h"
#include "DefaultSceneContext/DefaultSceneContext.h"
#include "Detachable/Detachable.h"
#include "UpdateTimeText/UpdateTimeText.h"
#include <ProjectSettings/ProjectSettings.h>
#include <core_widgets.h>

WidgetPtr WMainWindow::Build()
{
	UPtr const sceneContext{ UCreate<WDefaultSceneContext>{ "Scene Context" }(SProjectSettings::Get<std::string>("/startupScene")) };
	AddSceneContext(sceneContext);
	
	return (
		UCreate<WColumn>{ "Main Window Column" }()
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
			sceneContext
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
