#include "MainWindow/MainWindow.h"

#include "AssetExplorer/AssetExplorer.h"
#include "DefaultSceneContext/DefaultSceneContext.h"
#include "Detachable/Detachable.h"
#include "UpdateTimeText/UpdateTimeText.h"
#include <ProjectSettings/ProjectSettings.h>
#include <core_widgets.h>
#include <CoreWidgets/VerticalSplitter/VerticalSplitter.h>

WidgetPtr WMainWindow::Build()
{
	UPtr const sceneContext{ UCreate<WDefaultSceneContext>{ "Scene Context" }(UPath{ SProjectSettings::Get<std::string>("/startupScene") }) };
	AddSceneContext(sceneContext);
	
	return (
		UCreate<WVerticalSplitter>{ "Main Window Column" }()
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
			UCreate<WDetachable>{ "Asset Explorer" }()
			| Apply(&WDetachable::SetExpandWeight, glm::vec2{ 1.0f, 0.4f })
			| (
				UCreate<WAssetExplorer>{ "Asset Explorer" }()
			)
		)
	);
}

#include "MainWindow.generated.inl"
