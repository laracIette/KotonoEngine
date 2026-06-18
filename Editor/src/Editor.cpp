#include "Editor.h"
#include "Camera.h"
#include "MainWindow.h"
#include "Visualizer.h"
#include <kotono_platform/Window.h>
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
	mainWindow_ = UCreate<WMainWindow>{ "Main Window" }();
	mainWindow_->BeginDraw();
}

void SEditor::DeleteWidget()
{
	if (mainWindow_)
	{
		mainWindow_->EndDraw();
		mainWindow_->Delete();
	}
}

void SEditor::RefreshMainWindowWidget()
{
	DeleteWidget();
	CreateWidget();
}
