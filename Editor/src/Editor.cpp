#include "Editor.h"
#include "Camera.h"
#include "MainWindow.h"
#include "Visualizer.h"
#include <kotono_interface/Interface.h>
#include <kotono_timing/Timer.h>
#include <kotono_timing/TimerManager.h>

void GEditor::Init()
{
	Visualizer.Init();
	Camera.Init();
	Interface.Init();

	auto& updateTimer{ TimerManager.GetTimer("update time text") };
	updateTimer.SetDuration(1.0f / 20.0f);
	updateTimer.SetIsRepeat(true);
	updateTimer.Start();

	mainWindow_ = UCreate<WMainWindow>{ "Main Window" }();
	mainWindow_->BeginDraw();
}

void GEditor::Cleanup()
{
	Camera.Cleanup();

	if (mainWindow_)
	{
		mainWindow_->EndDraw();
		mainWindow_->Delete();
	}
}
