#include "Editor.h"
#include "Camera.h"
#include "MainWindow.h"
#include "Visualizer.h"
#include <kotono_platform/Window.h>
#include <kotono_platform/WindowViewport.h>
#include <kotono_timing/Timer.h>
#include <kotono_timing/TimerManager.h>
#include <kotono_interface/Interface.h>

void SEditor::Init()
{
	Visualizer.Init();
	Camera.Init();
	Interface.Init();

	auto& updateTimer{ TimerManager.GetTimer("update time text") };
	updateTimer.SetDuration(1.0f / 20.0f);
	updateTimer.SetIsRepeat(true);
	updateTimer.Start();

	CreateWidget();

	Window.GetEventWindowResized().AddListener(this, &SEditor::RefreshMainWindowWidget);
}

void SEditor::Update()
{
}

void SEditor::Cleanup()
{
	Camera.Cleanup();

	DeleteWidget();

	Window.GetEventWindowResized().RemoveListener(this, &SEditor::RefreshMainWindowWidget);
}

void SEditor::CreateWidget()
{
	widget_ = UCreate<WMainWindow>{ "Main Window" }();
	widget_->Display({
		.position = { 0.0f, 0.0f },
		.bounds = static_cast<glm::vec2>(WindowViewport.GetExtent()),
		.layer = 0,
		.scissor = { glm::zero<glm::ivec2>(), WindowViewport.GetExtent() },
	});
}

void SEditor::DeleteWidget()
{
	if (widget_)
	{
		widget_->Remove();
		widget_->Delete();
	}
}

void SEditor::RefreshMainWindowWidget()
{
	DeleteWidget();
	CreateWidget();
}
