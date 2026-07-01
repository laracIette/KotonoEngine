#include "Editor.h"
#include "Camera.h"
#include "MainWindow.h"
#include "Visualizer.h"
#include <kotono_platform/Window.h>
#include <kotono_timing/Timer.h>
#include <kotono_timing/TimerManager.h>
#include <kotono_interface/Interface.h>

void GEditor::Init()
{
	Visualizer.Init();
	Camera.Init();
	Interface.Init();

	auto& updateTimer{ TimerManager.GetTimer("update time text") };
	updateTimer.SetDuration(1.0f / 20.0f);
	updateTimer.SetIsRepeat(true);
	updateTimer.Start();

	CreateWidget();

	Window.GetEventWindowResized().AddListener(this, &GEditor::RefreshMainWindowWidget);
}

void GEditor::Update()
{
}

void GEditor::Cleanup()
{
	Camera.Cleanup();

	DeleteWidget();

	Window.GetEventWindowResized().RemoveListener(this, &GEditor::RefreshMainWindowWidget);
}

void GEditor::CreateWidget()
{
	mainWindow_ = UCreate<WMainWindow>{ "Main Window" }();
	mainWindow_->BeginDraw();
}

void GEditor::DeleteWidget()
{
	if (mainWindow_)
	{
		mainWindow_->EndDraw();
		mainWindow_->Delete();
	}
}

void GEditor::RefreshMainWindowWidget()
{
	DeleteWidget();
	CreateWidget();
}
