#include "EngineInterface.h"
#include "MainWindow.h"
#include <kotono_framework/Viewport.h>

void SEngineInterface::Init()
{
	widget_ = new WMainWindow();
	widget_->CacheBuild();
	widget_->Display({
		.position = { 0.0f, 0.0f },
		.bounds = static_cast<glm::vec2>(WindowViewport.GetExtent()),
		.layer = 0,
	});
}

void SEngineInterface::Cleanup()
{
	widget_->Cleanup();
	delete widget_;
}